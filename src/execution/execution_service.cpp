#include "execution/execution_service.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <memory>
#include <sstream>
#include <string>
#include <system_error>
#include <oatpp/core/Types.hpp>
#include <oatpp/encoding/Base64.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/script/lua_script_runner.hpp"
#include "execution/script/script_executor.hpp"
#include "network/dto/execution/request_dto.hpp"

namespace engine::execution
{
    CoreExecutionResult ExecutionService::execute(const oatpp::Object<network::dto::execution::RequestDto>& request)
    {
        auto temp_dir = std::filesystem::temp_directory_path() / "ogdf_engine_runs";
        std::filesystem::create_directories(temp_dir);

        auto file_extension = request->options->output_data_format->c_str();
        auto file_name = std::string(request->input_data->id->c_str()) + "." + file_extension;

        auto input_path = temp_dir / file_name;
        auto output_path = temp_dir / ("output_" + file_name);

        std::shared_ptr<void> guard(nullptr, [input_path, output_path](void*)
        {
            std::error_code error_code;

            if (std::filesystem::exists(input_path, error_code))
            {
                std::filesystem::remove(input_path, error_code);
            }

            if (std::filesystem::exists(output_path, error_code))
            {
                std::filesystem::remove(output_path, error_code);
            }
        });

        try
        {
            auto decoded_input = oatpp::encoding::Base64::decode(request->input_data->chunk_data);
            std::ofstream input_file(input_path, std::ios::binary);
            if (!input_file)
            {
                throw ExecutionServiceException("Cannot create temporary input file at path: {}", input_path.string());
            }
            input_file.write(decoded_input->data(), static_cast<std::streamsize>(decoded_input->size()));
            input_file.close();

            script::ScriptContext context;
            context.script_content = request->script->c_str();
            context.input_path = input_path;
            context.output_path = output_path;

            script::LuaScriptRunner runner;
            auto result = runner.run_from_string(context);

            if (!result.is_success())
            {
                return {
                    .status = result.status,
                    .error{
                        .type = result.error.type,
                        .message = result.error.message
                    }
                };
            }

            std::ifstream output_file(output_path, std::ios::binary);
            if (!output_file)
            {
                throw ExecutionServiceException("Cannot open temporary output file at path: {}", output_path.string());
            }

            std::ostringstream output_stream;
            output_stream << output_file.rdbuf();
            if (!output_file.good() && !output_file.eof())
            {
                throw ExecutionServiceException("Failed to read temporary output file at path: {}", output_path.string());
            }

            oatpp::String buffer = output_stream.str();
            auto encoded_output = oatpp::encoding::Base64::encode(buffer);

            return {
                .status = CoreExecutionStatus::Success,
                .output_data = encoded_output
            };
        } catch (const std::exception& e)
        {
            return {
                .status = CoreExecutionStatus::Failure,
                .error{
                    .type = CoreExecutionErrorType::Execution_Failed,
                    .message = e.what()
                }
            };
        }
    }
} // namespace engine::execution
