/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#include "execution/script/lua_script_runner.hpp"

#include <filesystem>
#include <sol/sol.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/reserved_keywords.hpp"
#include "execution/script/script_executor.hpp"
#include "utils/filesystem_utils.hpp"

namespace engine::execution::script
{
    sol::environment LuaScriptRunner::create_script_env(const ScriptContext& context) const
    {
        sol::environment env(lua_, sol::create, lua_.globals());

        if (context.input_path)
        {
            env[reserved::InputPath] = utils::filesystem::to_forward_slashes(*context.input_path);
        }

        if (context.output_path)
        {
            env[reserved::OutputPath] = utils::filesystem::to_forward_slashes(*context.output_path);
        }

        return env;
    }

    CoreExecutionResult LuaScriptRunner::run_from_string(const ScriptContext& context) const
    {
        const sol::environment env = create_script_env(context);
        const ScriptExecutor executor(lua_);
        return executor.execute_from_string(context.script_content, env);
    }

    CoreExecutionResult LuaScriptRunner::run_from_file(const ScriptContext& context) const
    {
        const sol::environment env = create_script_env(context);
        const ScriptExecutor executor(lua_);
        if (!std::filesystem::exists(context.script_content))
        {
            return {
                .status = CoreExecutionStatus::Failure,
                .error = {
                    .type = CoreExecutionErrorType::FileNotFound,
                    .message = "Script file not found"
                }
            };
        }
        return executor.execute_from_file(context.script_content, env);
    }
} // namespace engine::execution::script
