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
#include <filesystem>
#include <fstream>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/execution_service.hpp"
#include "network/dto/message_dto.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/dto/execution/discovery_dto.hpp"
#include "network/dto/execution/source_code_dto.hpp"
#include "network/mapper/execution_result_mapper.hpp"

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
         *
         * Generates and activates the initial API secret on server startup.
         * @param object_mapper The object mapper for DTO serialization.
         */
        explicit ExecutionController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& object_mapper)
            : ApiController(object_mapper)
        {
            const std::string secret = regenerate_api_secret();
            OATPP_LOGI("ExecutionController", "API secret initialized: %s", secret.c_str())
        }

        /**
         * @brief Endpoint info for health check.
         */
        ENDPOINT_INFO(health)
        {
            info->summary = "Check service health status";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json");
        }

        /**
         * @brief Health check endpoint.
         * @return 200 OK with a status message.
         */
        ENDPOINT("GET", "/api/health", health)
        {
            const auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = "OK";
            return createDtoResponse(Status::CODE_200, response);
        }

        /**
         * @brief Endpoint info for discovery of available methods and files.
         */
        ENDPOINT_INFO(discovery)
        {
            info->summary = "List available layout methods and graphml files";
            info->addResponse<Object<dto::execution::DiscoveryDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_401, "application/json", "Unauthorized - missing or invalid API secret");
        }

        /**
         * @brief Endpoint for discovering available methods and files.
         * @param http_request The incoming HTTP request used for API secret verification.
         * @return 200 OK with the discovery data, or 401 for unauthorized requests.
         */
        ENDPOINT("GET", "/api/discovery", discovery, REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, http_request))
        {
            if (!verify_api_secret(http_request))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 401;
                error->message = "Unauthorized - missing or invalid API secret";
                return createDtoResponse(Status::CODE_401, error);
            }

            auto discovery_dto = dto::execution::DiscoveryDto::createShared();
            discovery_dto->methods = oatpp::Vector<oatpp::String>::createShared();
            discovery_dto->files = oatpp::Vector<oatpp::String>::createShared();

            try
            {
                namespace fs = std::filesystem;
                
                // Scan C++ examples for method names
                if (fs::exists("examples/cpp"))
                {
                    for (const auto& entry : fs::directory_iterator("examples/cpp"))
                    {
                        if (entry.path().extension() == ".cpp")
                        {
                            discovery_dto->methods->push_back(entry.path().stem().string().c_str());
                        }
                    }
                }

                // Scan GraphML examples for file names
                if (fs::exists("examples/graphml"))
                {
                    for (const auto& entry : fs::directory_iterator("examples/graphml"))
                    {
                        if (entry.path().extension() == ".graphml")
                        {
                            discovery_dto->files->push_back(entry.path().filename().string().c_str());
                        }
                    }
                }
            }
            catch (const std::exception& e)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 500;
                error->message = "Internal server error during discovery: " + std::string(e.what());
                return createDtoResponse(Status::CODE_500, error);
            }

            return createDtoResponse(Status::CODE_200, discovery_dto);
        }

        /**
         * @brief Endpoint info for API secret generation.
         */
        ENDPOINT_INFO(generate_api_secret)
        {
            info->summary = "Generate a new random 32-digit API secret";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json", "New API secret was generated");
        }

        /**
         * @brief Endpoint for generating a new random 32-digit API secret.
         * @return 200 OK with the newly activated API secret.
         */
        ENDPOINT("POST", "/api/secret/generate", generate_api_secret)
        {
            const auto secret = regenerate_api_secret();
            OATPP_LOGI("ExecutionController", "New API secret generated: %s", secret.c_str())

            const auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = secret.c_str();
            return createDtoResponse(Status::CODE_200, response);
        }

        /**
         * @brief Endpoint info for retrieving source code of a method.
         */
        ENDPOINT_INFO(get_source)
        {
            info->summary = "Get C++ and Lua source code for a specific method";
            info->addResponse<Object<dto::execution::SourceCodeDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_404, "application/json", "Method not found");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_401, "application/json", "Unauthorized - missing or invalid API secret");
        }


        /**
         * @brief Endpoint for retrieving source code of a method.
         * @param http_request The incoming HTTP request used for API secret verification.
         * @param method The name of the layout method.
         * @return 200 OK with the source codes, 404 if method not found, or 401 for unauthorized requests.
         */
        ENDPOINT("GET", "/api/source/{method}", get_source, REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, http_request), PATH(String, method))
        {
            if (!verify_api_secret(http_request))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 401;
                error->message = "Unauthorized - missing or invalid API secret";
                return createDtoResponse(Status::CODE_401, error);
            }

            namespace fs = std::filesystem;
            std::string method_name = method->c_str();
            fs::path cpp_path = fs::path("examples/cpp") / (method_name + ".cpp");
            fs::path lua_path = fs::path("examples/lua") / (method_name + ".lua");

            if (!fs::exists(cpp_path) || !fs::exists(lua_path))
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 404;
                error->message = "Source code for method '" + method_name + "' not found";
                return createDtoResponse(Status::CODE_404, error);
            }

            auto source_dto = dto::execution::SourceCodeDto::createShared();
            
            auto read_file = [](const fs::path& path) {
                std::ifstream file(path);
                if (!file) return std::string();
                return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            };

            source_dto->cpp_code = read_file(cpp_path).c_str();
            source_dto->lua_code = read_file(lua_path).c_str();

            return createDtoResponse(Status::CODE_200, source_dto);
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
            if (!verify_api_secret(http_request))
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
         * @brief Verifies that an incoming request is authorized to use the protected API endpoints.
         *
         * A request is authorized only if its "X-Api-Secret" header matches the currently
         * active API secret. The comparison is performed in constant time to prevent timing attacks.
         * @param request The incoming HTTP request.
         * @return True if the request is authorized.
         */
        bool verify_api_secret(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request> &request) const
        {
            const oatpp::String provided_secret = request->getHeader("X-Api-Secret");

            if (!provided_secret)
            {
                return false;
            }

            std::lock_guard<std::mutex> lock(api_secret_mutex_);

            const std::string provided = provided_secret->c_str();

            if (provided.length() != api_secret_.length())
            {
                return false;
            }

            unsigned char difference = 0;

            for (std::size_t i = 0; i < api_secret_.length(); ++i)
            {
                difference |= static_cast<unsigned char>(provided[i]) ^ static_cast<unsigned char>(api_secret_[i]);
            }

            return difference == 0;
        }

    private:
        /**
         * @brief The length of the generated API secret.
         */
        static constexpr std::size_t API_SECRET_LENGTH = 32;

        /**
         * @brief Generates a new random 32-digit API secret and activates it.
         * @return The newly activated API secret.
         */
        std::string regenerate_api_secret()
        {
            thread_local std::mt19937_64 generator(std::random_device{}());
            std::uniform_int_distribution<int> digit_distribution(0, 9);

            std::string secret;
            secret.reserve(API_SECRET_LENGTH);

            for (std::size_t i = 0; i < API_SECRET_LENGTH; ++i)
            {
                secret += static_cast<char>('0' + digit_distribution(generator));
            }

            {
                std::lock_guard<std::mutex> lock(api_secret_mutex_);
                api_secret_ = secret;
            }

            return secret;
        }

        /**
         * @brief The mutex guarding the active API secret.
         */
        mutable std::mutex api_secret_mutex_;

        /**
         * @brief The currently active API secret.
         */
        std::string api_secret_;
    };
} // namespace engine::network::api

#include OATPP_CODEGEN_END(ApiController)
