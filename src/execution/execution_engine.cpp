#include "execution/execution_engine.hpp"
#include "execution/script/script_executor.hpp"

namespace execution {

    interop::types::ExecutionResponse ExecutionEngine::execute_script(const std::string& script) const
    {
        if (script.empty()) {
          return interop::types::ExecutionResponse(
            interop::types::ExecutionStatus::Failure,
            nullptr,
            interop::types::ExecutionError(interop::types::ExecutionErrorType::Invalid_Input, "Script content cannot be empty"),
            {});
        }

        if (m_execution_type != interop::types::ExecutionType::Lua_Script) {
          return interop::types::ExecutionResponse(
            interop::types::ExecutionStatus::Failure,
            nullptr,
            interop::types::ExecutionError(interop::types::ExecutionErrorType::Invalid_Argument, "Execution engine not configured for script execution"),
            {});
        }

        return script::ScriptExecutor::execute_from_string(script);
    }

    interop::types::ExecutionResponse ExecutionEngine::execute_script_file(const std::filesystem::path& path) const
    {
        if (!is_valid_path(path)) {
          return interop::types::ExecutionResponse(
            interop::types::ExecutionStatus::Failure,
            nullptr,
            interop::types::ExecutionError(interop::types::ExecutionErrorType::File_Not_Found, "Invalid or missing script file"),
            {});
        }

        if (m_execution_type != interop::types::ExecutionType::Lua_Script) {
          return interop::types::ExecutionResponse(
            interop::types::ExecutionStatus::Failure,
            nullptr,
            interop::types::ExecutionError(interop::types::ExecutionErrorType::Invalid_Argument, "Execution engine not configured for script execution"),
            {});
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

} // namespace execution
