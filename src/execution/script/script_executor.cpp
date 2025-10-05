#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include "bindings/lua/registry.hpp"
#include "utils/response_factory.hpp"

#include <sol/sol.hpp>

namespace engine::execution::script
{

    namespace
    {
        utils::ExecutionResponsePtr execute_lua(const std::function<sol::protected_function_result()>& script_runner)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            try
            {
                const auto lua_result = script_runner();
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
                return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                            interop::types::ExecutionErrorType::Execution_Failed,
                                                            e.what());
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration_milliseconds =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            return utils::ResponseFactory::create_success(duration_milliseconds);
        }
    } // namespace

    utils::ExecutionResponsePtr ScriptExecutor::execute_from_file(const std::filesystem::path& script_path)
    {
        auto& lua = bindings::lua::Registry::instance().lua();

        const auto normalized_path = normalize_path(script_path);

        return execute_lua([&] { return lua.safe_script_file(normalized_path.string()); });
    }

    utils::ExecutionResponsePtr ScriptExecutor::execute_from_string(const std::string& script_content)
    {
        auto& lua = bindings::lua::Registry::instance().lua();

        return execute_lua([&] { return lua.safe_script(script_content); });
    }

    fs::path ScriptExecutor::normalize_path(const fs::path& path)
    {
        auto normalized = path.string();
        std::ranges::replace(normalized, '\\', '/');
        return fs::absolute(normalized);
    }

} // namespace engine::execution::script
