#include "execution/execution_service.hpp"

#include "execution/execution_result.hpp"
#include "execution/script/script_executor.hpp"
#include "execution/execution_engine.hpp"
#include "execution/reserved_keywords.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "utils/filesystem_utils.hpp"
#include "utils/string_utils.hpp"

#include <oatpp/core/Types.hpp>
#include <oatpp/encoding/Base64.hpp>

namespace engine::execution
{
    namespace
    {
        std::string prepare_script(const std::string& script, const fs::path& input_path, const fs::path& output_path)
        {
            std::string script_content = script;
            utils::string::replace_all(script_content, reserved::INPUT_PATH, utils::filesystem::to_forward_slashes(input_path));
            utils::string::replace_all(script_content, reserved::OUTPUT_PATH, utils::filesystem::to_forward_slashes(output_path));
            return script_content;
        }
    }

    ExecutionResult ExecutionService::execute(const oatpp::Object<network::dto::execution::RequestDto>& request)
    {
        auto temp_dir = fs::temp_directory_path() / "ogdf_engine_runs";
        fs::create_directories(temp_dir);

        std::string file_extension = request->options->output_data_format->c_str();
        std::string file_name = std::string(request->input_data->id->c_str()) + "." + file_extension;

        auto input_path = temp_dir / file_name;
        auto output_path = temp_dir / ("output_" + file_name);

        std::shared_ptr<void> guard(nullptr, [&](void*)
        {
            std::error_code error_code;
            fs::remove(input_path, error_code);
            fs::remove(output_path, error_code);
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

            auto prepared_script = prepare_script(request->script->c_str(), input_path, output_path);

            auto response = ExecutionEngine::execute(interop::types::Lua_Script, prepared_script);

            if (response->status != interop::types::Success)
            {
                return {
                    false, nullptr, response->error.type,
                    response->error.message ? response->error.message : "Unknown execution error"
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

            return {true, encoded_output, {}};
        } catch (const std::exception& e)
        {
            return {false, nullptr, interop::types::ExecutionErrorType::Execution_Failed, e.what()};
        }
    }
}
