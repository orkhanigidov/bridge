/**
 * Project: Engine
 * File: execution_controller.hpp
 *
 * Copyright (C) 2025-2026 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file execution_controller.hpp
 * @brief Declares the ExecutionController API controller for execution-related endpoints.
 */

#pragma once

#include <exception>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/execution_service.hpp"
#include "network/dto/message_dto.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/mapper/execution_result_mapper.hpp"
#include "execution/script/cpp_script_runner.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "execution/script/lua_state_manager.hpp"
#include "bindings/lua/lua_binder.hpp"
#include "network/dto/execution/sequential_dto.hpp"
#include "utils/temp_file_guard.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace engine::network::api
{
    /**
     * @class ExecutionController
     * @brief API controller for handling execution-related HTTP requests.
     */
    class ExecutionController final : public oatpp::web::server::api::ApiController
    {
    public:
        /**
         * @brief Constructor.
         * Loads the API secret from the environment or ".env".
         * @param object_mapper The object mapper for DTO serialization.
         */
        explicit ExecutionController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& object_mapper)
            : ApiController(object_mapper)
        {
            const std::string secret = load_api_secret();

            if (secret.length() >= MIN_API_SECRET_LENGTH)
            {
                std::lock_guard<std::mutex> lock(api_secret_mutex_);
                api_secret_ = secret;
                secret_configured_ = true;
                OATPP_LOGI("ExecutionController", "API secret loaded (%zu characters)", secret.length())
            }
            else
            {
                OATPP_LOGE("ExecutionController", "No valid BRIDGE_API_SECRET found (environment or .env) - protected endpoints will return 401")
            }
        }

        /**
         * @brief Endpoint info for health check.
         */
        ENDPOINT_INFO(health)
        {
            info->summary = "Check service health status";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_401, "application/json", "Unauthorized");
        }

        /**
         * @brief Health check endpoint. Requires the API secret so the frontend
         * can verify both connectivity and authorization in one call.
         * @return 200 OK with a status message, or 401 if the secret is missing or invalid.
         */
        ENDPOINT("GET", "/api/health", health, REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, http_request))
        {
            if (!verify_api_secret(http_request, "health"))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 401;
                error->message = "Unauthorized - missing or invalid API secret";
                return createDtoResponse(Status::CODE_401, error);
            }

            const auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = "OK";
            return createDtoResponse(Status::CODE_200, response);
        }

        ENDPOINT_INFO(execute_sequential)
        {
            info->summary = "Execute C++ and Lua scripts sequentially and return their SVG outputs";
            info->addConsumes<Object<dto::execution::SequentialRequestDto>>("application/json");
            info->addResponse<Object<dto::execution::SequentialResponseDto>>(Status::CODE_200, "application/json", "Both scripts executed successfully");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_400, "application/json", "Invalid request");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_401, "application/json", "Unauthorized");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_500, "application/json", "Execution failed");
        }

        ENDPOINT("POST", "/api/execute_sequential", execute_sequential, REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, http_request), BODY_DTO(Object<dto::execution::SequentialRequestDto>, request))
        {
            if (!verify_api_secret(http_request, "execute_sequential"))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 401;
                error->message = "Unauthorized - missing or invalid API secret";
                return createDtoResponse(Status::CODE_401, error);
            }

            if (!request || !request->cpp_code || !request->lua_code || !request->graphml_name || !request->graphml_file)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 400;
                error->message = "cpp_code, lua_code, graphml_name, and graphml_file are required";
                return createDtoResponse(Status::CODE_400, error);
            }

            try
            {
                namespace fs = std::filesystem;

                const fs::path graphml_name(request->graphml_name->c_str());
                if (graphml_name.filename() != graphml_name || graphml_name.extension() != ".graphml")
                {
                    const auto error = dto::MessageDto::createShared();
                    error->status_code = 400;
                    error->message = "graphml_name must be a .graphml filename";
                    return createDtoResponse(Status::CODE_400, error);
                }

                const std::string graphml_content = request->graphml_file->c_str();

                if (graphml_content.find("<graphml") == std::string::npos)
                {
                    const auto error = dto::MessageDto::createShared();
                    error->status_code = 400;
                    error->message = "graphml_file must contain GraphML document content";
                    return createDtoResponse(Status::CODE_400, error);
                }

                const fs::path temp_root = fs::absolute("temp");
                fs::create_directories(temp_root);

                fs::path run_root;
                do
                {
                    run_root = temp_root / std::to_string(std::random_device{}());
                } while (!fs::create_directory(run_root));

                const utils::TempDirectoryGuard run_dir_guard(run_root);

                // Run each script in its own directory with the input under its original name.
                const fs::path cpp_dir = run_root / "cpp";
                const fs::path lua_dir = run_root / "lua";
                fs::create_directories(cpp_dir);
                fs::create_directories(lua_dir);

                const auto write_input = [&graphml_content, &graphml_name](const fs::path& dir)
                {
                    const fs::path input_path = dir / graphml_name;
                    std::ofstream input_file(input_path, std::ios::binary);

                    if (!input_file || !input_file.write(graphml_content.data(), static_cast<std::streamsize>(graphml_content.size())))
                    {
                        throw std::runtime_error("Could not write input GraphML file: " + input_path.string());
                    }
                };

                write_input(cpp_dir);
                write_input(lua_dir);

                const auto quoted_path = [](const fs::path &path)
                {
                    std::string value = path.generic_string();
                    std::string escaped;
                    escaped.reserve(value.size());

                    for (const char ch : value)
                    {
                        if (ch == '\\' || ch == '"')
                        {
                            escaped.push_back('\\');
                        }
                        escaped.push_back(ch);
                    }

                    return "\"" + escaped + "\"";
                };

                const auto replace_all = [](std::string& text, const std::string& placeholder, const std::string& replacement)
                {
                    std::size_t position = 0;
                    while ((position = text.find(placeholder, position)) != std::string::npos)
                    {
                        text.replace(position, placeholder.size(), replacement);
                        position += replacement.size();
                    }
                };

                std::string cpp_code = request->cpp_code->c_str();
                std::string lua_code = request->lua_code->c_str();

                replace_all(cpp_code, "__input__", quoted_path(cpp_dir / graphml_name));
                replace_all(cpp_code, "__output__", quoted_path(cpp_dir));

                replace_all(lua_code, "__input__", quoted_path(lua_dir / graphml_name));
                replace_all(lua_code, "__output__", quoted_path(lua_dir));

                execution::script::ScriptContext cpp_context;
                cpp_context.script_content = cpp_code;
                cpp_context.input_path = cpp_dir / graphml_name;
                cpp_context.output_path = cpp_dir;

                execution::script::CppScriptRunner cpp_runner;
                const auto cpp_result = cpp_runner.run_from_string(cpp_context);

                if (!cpp_result.is_success())
                {
                    throw std::runtime_error("C++ execution failed: " + cpp_result.error.message);
                }

                execution::script::ScriptContext lua_context;
                lua_context.script_content = lua_code;
                lua_context.input_path = lua_dir / graphml_name;
                lua_context.output_path = lua_dir;

                execution::script::LuaScriptRunner lua_runner(execution::script::LuaStateManager::get_state());

                // Lua runs in-process: run it from its sandbox so relative file names
                // resolve there. The mutex serializes the cwd switch between requests.
                static std::mutex lua_cwd_mutex;
                std::lock_guard<std::mutex> cwd_lock(lua_cwd_mutex);

                const fs::path previous_cwd = fs::current_path();

                try
                {
                    fs::current_path(lua_dir);

                    const auto lua_result = lua_runner.run_from_string(lua_context);

                    fs::current_path(previous_cwd);

                    if (!lua_result.is_success())
                    {
                        throw std::runtime_error("Lua execution failed: " + lua_result.error.message);
                    }
                }
                catch (...)
                {
                    std::error_code restore_error;
                    fs::current_path(previous_cwd, restore_error);
                    throw;
                }

                const auto read_file = [](const fs::path &path)
                {
                    std::ifstream file(path, std::ios::binary);
                    if (!file)
                    {
                        throw std::runtime_error("Output file was not created: " + path.string());
                    }

                    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
                };

                // Pick up whatever output the script produced (extension-wise).
                // The input file is excluded so it is never returned as an output.
                const auto find_output = [](const fs::path& dir, const std::string& extension, const std::string& exclude_name = {})
                {
                    fs::path best;

                    for (const auto& entry : fs::directory_iterator(dir))
                    {
                        const fs::path& path = entry.path();

                        if (!entry.is_regular_file() || path.extension().string() != extension || path.filename().string() == exclude_name)
                        {
                            continue;
                        }

                        if (best.empty() || path.string() < best.string())
                        {
                            best = path;
                        }
                    }

                    if (best.empty())
                    {
                        throw std::runtime_error("No " + extension + " output was produced in " + dir.string());
                    }

                    return best;
                };

                const std::string cpp_svg = read_file(find_output(cpp_dir, ".svg"));
                const std::string lua_svg = read_file(find_output(lua_dir, ".svg"));
                const std::string cpp_graphml_content = read_file(find_output(cpp_dir, ".graphml", graphml_name.string()));
                const std::string lua_graphml_content = read_file(find_output(lua_dir, ".graphml", graphml_name.string()));

                auto response = dto::execution::SequentialResponseDto::createShared();
                response->cpp_svg = cpp_svg.c_str();
                response->lua_svg = lua_svg.c_str();
                response->cpp_graphml = cpp_graphml_content.c_str();
                response->lua_graphml = lua_graphml_content.c_str();

                return createDtoResponse(Status::CODE_200, response);
            }
            catch (const std::exception& e)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 500;
                error->message = "Sequential execution failed: " + std::string(e.what());
                return createDtoResponse(Status::CODE_500, error);
            }
        }

        /**
         * @brief Endpoint info for script or pipeline execution.
         */
        ENDPOINT_INFO(execution)
        {
            info->summary = "Execute a script or pipeline";
            info->addConsumes<oatpp::Object<dto::execution::RequestDto>>("application/json");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_200, "application/json", "Execution was successful");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_500, "application/json", "Execution failed");
            info->addResponse<oatpp::Object<dto::MessageDto>>(Status::CODE_400, "application/json", "Invalid request body");
            info->addResponse<oatpp::Object<dto::MessageDto>>(Status::CODE_401, "application/json", "Unauthorized - missing or invalid API secret");
        }

        /**
         * @brief Endpoint for executing a script or pipeline.
         * @param http_request The incoming HTTP request used for API secret verification.
         * @param request The execution request DTO.
         * @return 200 OK with an execution result, 400 for invalid request, 401 for unauthorized requests, or 500 for errors.
         */
        ENDPOINT("POST", "/api/execute_script", execution, REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, http_request), BODY_DTO(Object<dto::execution::RequestDto>, request))
        {
            if (!verify_api_secret(http_request, "execute_script"))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 401;
                error->message = "Unauthorized - missing or invalid API secret";
                return createDtoResponse(Status::CODE_401, error);
            }

            if (!request)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 400;
                error->message = "Request body is missing or malformed";
                return createDtoResponse(Status::CODE_400, error);
            }

            try
            {
                const auto result = execution::ExecutionService::execute(request);
                const auto response_dto = mapper::ExecutionResultMapper::to_dto(result);
                const auto status = result.is_success() ? Status::CODE_200 : Status::CODE_500;
                return createDtoResponse(status, response_dto);
            } catch (const std::exception& e)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 500;
                error->message = "An unexpected error occurred: " + std::string(e.what());
                return createDtoResponse(Status::CODE_500, error);
            }
        }

        /**
         * @brief Verifies the API secret for a request.
         * @param request The incoming request.
         * @param endpoint The endpoint name.
         * @return True if authorized.
         */
        bool verify_api_secret(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request> &request, const char *endpoint) const
        {
            bool authorized = false;

            if (secret_configured_)
            {
                const oatpp::String provided_secret = request->getHeader("X-Api-Secret");

                if (provided_secret)
                {
                    std::lock_guard<std::mutex> lock(api_secret_mutex_);
                    const std::string provided = provided_secret->c_str();

                    if (provided.length() == api_secret_.length())
                    {
                        unsigned char difference = 0;

                        for (std::size_t i = 0; i < api_secret_.length(); ++i)
                        {
                            difference |= static_cast<unsigned char>(provided[i]) ^ static_cast<unsigned char>(api_secret_[i]);
                        }

                        authorized = difference == 0;
                    }
                }
            }

            OATPP_LOGI("ExecutionController", "Auth %s - endpoint: %s", authorized ? "granted" : "denied", endpoint)

            return authorized;
        }

    private:
        /**
         * @brief The minimum required length of the API secret.
         */
        static constexpr std::size_t MIN_API_SECRET_LENGTH = 32;

        /**
         * @brief Loads the API secret from the environment or ".env".
         * @return The secret, or an empty string if unavailable.
         */
        static std::string load_api_secret()
        {
            if (const char *env_secret = std::getenv("BRIDGE_API_SECRET"))
            {
                return env_secret;
            }

            std::ifstream env_file(".env");

            if (!env_file)
            {
                return {};
            }

            constexpr const char *KEY = "BRIDGE_API_SECRET=";
            const auto is_space = [](char ch)
            { return ch == ' ' || ch == '\t' || ch == '\r'; };

            std::string line;

            while (std::getline(env_file, line))
            {
                if (line.rfind(KEY, 0) != 0)
                {
                    continue; // comments, blank lines and other keys are skipped
                }

                std::string value = line.substr(std::strlen(KEY));

                while (!value.empty() && is_space(value.front()))
                {
                    value.erase(value.begin());
                }

                while (!value.empty() && is_space(value.back()))
                {
                    value.pop_back();
                }

                if (value.size() >= 2 && (value.front() == '"' || value.front() == '\'') && value.back() == value.front())
                {
                    value = value.substr(1, value.size() - 2); // optional quotes
                }

                return value;
            }

            return {};
        }

        /**
         * @brief The mutex guarding the active API secret.
         */
        mutable std::mutex api_secret_mutex_;

        /**
         * @brief The active API secret loaded at startup.
         */
        std::string api_secret_;

        /**
         * @brief Indicates whether a valid API secret was configured at startup.
         */
        bool secret_configured_ = false;
    };
} // namespace engine::network::api

#include OATPP_CODEGEN_END(ApiController)
