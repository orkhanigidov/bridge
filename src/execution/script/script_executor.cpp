#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include "bindings/lua/lua_binder.hpp"
#include "utils/filesystem_utils.hpp"
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
                const auto& lua_result = script_runner();
                if (!lua_result.valid())
                {
                    const sol::error error = lua_result;
                    return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                                interop::types::ExecutionErrorType::Execution_Failed,
                                                                error.what());
                }
            } catch (const sol::error& e)
            {
                return utils::ResponseFactory::create_error(interop::types::ExecutionStatus::Failure,
                                                            interop::types::ExecutionErrorType::Execution_Failed,
                                                            e.what());
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            return utils::ResponseFactory::create_success(static_cast<uint64_t>(duration_milliseconds));
        }
    }

    ScriptExecutor::ScriptExecutor()
    {
        lua_ = sol::state();
        lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os,
                            sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::utf8);

        bindings::lua::LuaBinder::register_bindings(lua_);
    }

    utils::ExecutionResponsePtr ScriptExecutor::execute_from_file(const std::filesystem::path& script_path)
    {
        const auto normalized_path = utils::filesystem::to_forward_slashes(fs::absolute(script_path));
        return execute_lua([&] { return lua_.safe_script_file(normalized_path); });
    }

    utils::ExecutionResponsePtr ScriptExecutor::execute_from_string(const std::string& script_content)
    {
        return execute_lua([&] { return lua_.safe_script(script_content); });
    }
} // namespace engine::execution::script
