#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include "bindings/lua/lua_binder.hpp"
#include "utils/filesystem_utils.hpp"

#include <sol/sol.hpp>

namespace
{
    engine::execution::CoreExecutionResult execute_lua(const std::function<sol::protected_function_result()>& script_runner)
    {
        const auto start = std::chrono::high_resolution_clock::now();

        try
        {
            const auto& lua_result = script_runner();
            if (!lua_result.valid())
            {
                const sol::error error = lua_result;
                return {
                    .status = engine::execution::CoreExecutionStatus::Failure,
                    .error
                    {
                        .type = engine::execution::CoreExecutionErrorType::Execution_Failed,
                        .message = error.what()
                    }
                };
            }
        } catch (const sol::error& e)
        {
            return {
                .status = engine::execution::CoreExecutionStatus::Failure,
                .error
                {
                    .type = engine::execution::CoreExecutionErrorType::Execution_Failed,
                    .message = e.what()
                }
            };
        }

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        return {
            .status = engine::execution::CoreExecutionStatus::Success,
            .metadata{
                .duration_milliseconds = duration_milliseconds
            }
        };
    }
}

namespace engine::execution::script
{
    ScriptExecutor::ScriptExecutor()
    {
        lua_ = sol::state();
        lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os,
                            sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::utf8);

        bindings::lua::LuaBinder::register_bindings(lua_);
    }

    CoreExecutionResult ScriptExecutor::execute_from_file(const std::filesystem::path& script_path)
    {
        const auto normalized_path = utils::filesystem::to_forward_slashes(fs::absolute(script_path));
        return execute_lua([&] { return lua_.safe_script_file(normalized_path); });
    }

    CoreExecutionResult ScriptExecutor::execute_from_string(const std::string& script_content)
    {
        return execute_lua([&] { return lua_.safe_script(script_content); });
    }
} // namespace engine::execution::script
