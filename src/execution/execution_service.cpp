/**
 * Project: Engine
 * File: execution_service.cpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file execution_service.cpp
 * @brief Implements the ExecutionService utility for executing requests.
 */

#include "execution/execution_service.hpp"

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <vector>
#include <oatpp/core/Types.hpp>
#include <oatpp/encoding/Base64.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "execution/script/script_executor.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "utils/temp_file_guard.hpp"

namespace engine::execution
{
    /**
     * @brief Executes a request by decoding input data, running a Lua script, and encoding the output. Handles all errors by returning a failure result or throwing ExecutionServiceException.
     * @param request The execution request DTO.
     * @return The result of the execution.
     * @throws ExecutionServiceExceeption on input/output errors.
     */
    CoreExecutionResult ExecutionService::execute(const oatpp::Object<network::dto::execution::RequestDto>& request)
    {
        auto temp_dir = std::filesystem::temp_directory_path() / "ogdf_engine_runs";
        std::filesystem::create_directories(temp_dir);

        if (!request->input_data || request->input_data->empty())
        {
            throw ExecutionServiceException("Input data is missing or empty");
        }

        auto file_extension = request->options->output_data_format->c_str();
        auto file_name = std::string(*request->input_data[0]->id) + "." + file_extension;

        auto input_path = temp_dir / file_name;
        auto output_path = temp_dir / ("output_" + file_name);

        utils::TempFileGuard input_file_guard(input_path);
        utils::TempFileGuard output_file_guard(output_path);

        try
        {
            const auto& chunks = request->input_data;
            std::vector sorted_chunks(chunks->begin(), chunks->end());
            std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](const auto& a, const auto& b)
            {
                return a->chunk_index < b->chunk_index;
            });

            std::ofstream input_file(input_path, std::ios::binary);
            if (!input_file)
            {
                throw ExecutionServiceException("Cannot create temporary input file at path: " + input_path.string());
            }

            for (const auto& chunk : sorted_chunks)
            {
                auto decoded_input = oatpp::encoding::Base64::decode(chunk->chunk_data);
                input_file.write(decoded_input->data(), static_cast<std::streamsize>(decoded_input->size()));
            }
            input_file.close();

            script::ScriptContext context;
            context.script_content = *request->script;
            context.input_path = input_path;
            context.output_path = output_path;

            script::LuaScriptRunner runner;
            auto result = runner.run_from_string(context);
            if (!result.is_success())
            {
                return result;
            }

            if (std::ifstream output_file(output_path, std::ios::binary); output_file)
            {
                oatpp::String encoded_output;
                std::ostringstream output_stream;
                output_stream << output_file.rdbuf();

                if (!output_file.good() && !output_file.eof())
                {
                    throw ExecutionServiceException("Failed to read temporary output file at path: " + output_path.string());
                }

                if (oatpp::String buffer = output_stream.str();
                    buffer && !buffer->empty())
                {
                    encoded_output = oatpp::encoding::Base64::encode(buffer);
                }

                result.output_data = encoded_output;
            }
            return result;
        } catch (const std::exception& e)
        {
            return {
                CoreExecutionStatus::Failure,
                {
                    CoreExecutionErrorType::ExecutionFailed,
                    e.what()
                }
            };
        }
    }
} // namespace engine::execution
