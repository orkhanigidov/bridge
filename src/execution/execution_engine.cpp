#include "execution/execution_engine.hpp"

#include "execution/script/script_executor.hpp"
#include "utils/response_factory.hpp"

namespace engine::execution {

    std::unique_ptr<interop::types::ExecutionResponse> ExecutionEngine::execute_script(const std::string& script) const
    {
        if (script.empty()) {
          return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Invalid_Argument, "Script is empty");
        }

        if (m_execution_type != interop::types::ExecutionType::Lua_Script) {
          return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Invalid_Argument, "Invalid execution type");
        }

        return script::ScriptExecutor::execute_from_string(script);
    }

    std::unique_ptr<interop::types::ExecutionResponse> ExecutionEngine::execute_script_file(const std::filesystem::path& path) const
    {
        if (!is_valid_path(path)) {
          return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Invalid_Argument, "Invalid file path");
        }

        if (m_execution_type != interop::types::ExecutionType::Lua_Script) {
          return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Invalid_Argument, "Invalid execution type");
        }

        return script::ScriptExecutor::execute_from_file(path);
    }

    bool ExecutionEngine::is_valid_path(const fs::path& path) {
        if (!fs::exists(path)) {
            std::cerr << "File does not exist: " << path << std::endl;
            return false;
        }

        if (!fs::is_regular_file(path)) {
            std::cerr << "File is not a regular file: " << path << std::endl;
            return false;
        }
        return true;
    }

} // namespace engine::execution
