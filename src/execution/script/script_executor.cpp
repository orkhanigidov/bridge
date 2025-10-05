#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include "bindings/lua/registry.hpp"
#include "utils/response_factory.hpp"

#include <sol/sol.hpp>

namespace engine::execution::script {

    std::unique_ptr<interop::types::ExecutionResponse> ScriptExecutor::execute_from_file(const std::filesystem::path& script_path)
    {
        const auto normalized_path = normalize_path(script_path);

        auto& lua        = bindings::lua::Registry::instance().lua();
        const auto start = std::chrono::high_resolution_clock::now();

        try
        {
            const auto lua_result = lua.safe_script_file(normalized_path.string());
            if (!lua_result.valid())
            {
                const sol::error error = lua_result;
                return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Execution_Failed, error.what());
            }
        }
        catch (const sol::error& e)
        {
            utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure, interop::types::ExecutionErrorType::Execution_Failed, e.what());
        }

        const auto end                         = std::chrono::high_resolution_clock::now();
        const auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        return utils::ResponseFactory::create_success(duration_milliseconds);
    }

    std::unique_ptr<interop::types::ExecutionResponse> ScriptExecutor::execute_from_string(const std::string& script_content)
    {
        auto& lua = bindings::lua::Registry::instance().lua();
        const auto start = std::chrono::high_resolution_clock::now();

        try
        {
            const auto lua_result = lua.safe_script(script_content);
            if (!lua_result.valid())
            {
                const sol::error error = lua_result;
                return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                            interop::types::ExecutionErrorType::Execution_Failed,
                                                            error.what());
            }
        }
        catch (const sol::error& e)
        {
            utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                 interop::types::ExecutionErrorType::Execution_Failed, e.what());
        }

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        return utils::ResponseFactory::create_success(duration_milliseconds);
    }

    fs::path ScriptExecutor::normalize_path(const fs::path& path)
    {
        auto normalized = path.string();
        std::ranges::replace(normalized, '\\', '/');
        return fs::absolute(normalized);
    }

} // namespace engine::execution::script
