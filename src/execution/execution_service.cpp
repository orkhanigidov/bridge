#include "execution/execution_service.hpp"

#include "execution/execution_error.hpp"
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
    ExecutionResult ExecutionService::execute(const oatpp::Object<network::dto::execution::RequestDto>& request)
    {
        auto temp_dir = fs::temp_directory_path() / "ogdf";
        fs::create_directories(temp_dir);

        std::string file_extension = request->options->output_data_format->c_str();
        std::string file_name = std::string(request->input_data->id->c_str()) + "." + file_extension;

        auto input_path = temp_dir / file_name;
        auto output_path = temp_dir / ("output_" + file_name);

        std::shared_ptr<void> guard(nullptr, [&](void*)
        {
            std::error_code error_code;

            if (fs::exists(input_path, error_code))
            {
                fs::remove(input_path, error_code);
            }

            if (fs::exists(output_path, error_code))
            {
                fs::remove(output_path, error_code);
            }
        });

        try
        {
            auto decoded_input = oatpp::encoding::Base64::decode(request->input_data->chunk_data);
            std::ofstream input_file(input_path, std::ios::binary);
            if (!input_file)
            {
                throw std::runtime_error("Cannot create temporary input file.");
            }
            input_file.write(decoded_input->data(), static_cast<std::streamsize>(decoded_input->size()));
            input_file.close();

            auto prepared_script = prepare_script(request->script->c_str(), input_path, output_path);

            ExecutionEngine engine;
            auto response = engine.execute(interop::types::Lua_Script, prepared_script);

            if (response->status != interop::types::Success)
            {
                return {
                    false, nullptr, interop::types::Execution_Failed,
                    response->error.message ? response->error.message : "Unknown execution error"
                };
            }

            std::ifstream output_file(output_path, std::ios::binary | std::ios::ate);
            if (!output_file)
            {
                throw std::runtime_error("Cannot open temporary output file.");
            }

            std::streamsize size = output_file.tellg();
            output_file.seekg(0, std::ios::beg);
            oatpp::String buffer(size);
            if (!output_file.read(buffer->data(), size))
            {
                throw std::runtime_error("Failed to read output file.");
            }

            auto encoded_output = oatpp::encoding::Base64::encode(buffer);

            return {true, encoded_output, {}};
        }
        catch (const std::exception& e)
        {
            return {false, nullptr, interop::types::ExecutionErrorType::Execution_Failed, e.what()};
        }
    }

    std::string ExecutionService::prepare_script(const std::string& script, const fs::path& input_path, const fs::path& output_path)
    {
        std::string script_content = script;
        utils::string::replace_all(script_content, Reserved::INPUT_PATH,
                                   utils::filesystem::to_forward_slashes(input_path));
        utils::string::replace_all(script_content, Reserved::OUTPUT_PATH,
                                   utils::filesystem::to_forward_slashes(output_path));
        return script_content;
    }
}
