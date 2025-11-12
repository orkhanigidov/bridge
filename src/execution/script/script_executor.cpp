/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file script_executor.cpp
 * @brief Implements the ScriptExecutor utility for executing Lua scripts.
 */

#include "execution/script/script_executor.hpp"

#define SOL_ALL_SAFETIES_ON 1

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <sol/sol.hpp>

#include "execution/core_execution_result.hpp"
#include "utils/filesystem_utils.hpp"

namespace
{
    /**
     * @brief Executes a Lua script using the provided runner and measures execution time. Returns a CoreExecutionResult with status and error/metadata.
     * @tparam Func The callable type for the script runner.
     * @param script_runner The function that runs the Lua script and returns a protected result.
     * @return The result of the script execution.
     */
    template <typename Func>
    engine::execution::CoreExecutionResult execute_lua(Func script_runner)
    {
        const auto start = std::chrono::high_resolution_clock::now();

        try
        {
            const sol::protected_function_result result = script_runner();
            if (!result.valid())
            {
                const sol::error err = result;
                const sol::call_status status = result.status();
                return {
                    engine::execution::CoreExecutionStatus::Failure,
                    {
                        engine::execution::CoreExecutionErrorType::ExecutionFailed,
                        sol::to_string(status) + ": " + err.what()
                    }
                };
            }
        } catch (const sol::error& e)
        {
            return {
                engine::execution::CoreExecutionStatus::Failure,
                {
                    engine::execution::CoreExecutionErrorType::ExecutionFailed,
                    e.what()
                }
            };
        }

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        return {
            engine::execution::CoreExecutionStatus::Success,
            {},
            {
                duration_milliseconds
            }
        };
    }
}

namespace engine::execution::script
{
    /**
     * @brief Executes a Lua script from a file using the given environment.
     * @param script_path The path to the Lua script file.
     * @param env The Lua environment to use.
     * @return The result of the script execution.
     */
    CoreExecutionResult ScriptExecutor::execute_from_file(const std::filesystem::path& script_path, const sol::environment& env) const
    {
        const auto normalized_path = utils::filesystem::to_forward_slashes(std::filesystem::absolute(script_path));

        return execute_lua([this, &normalized_path, &env]
        {
            return lua_.safe_script_file(normalized_path, env, sol::script_pass_on_error);
        });
    }

    /**
     * @brief Executes a Lua script from a sting using the given environment.
     * @param script_content The Lua script content as a string.
     * @param env The Lua environment to use.
     * @return The result of the script exection.
     */
    CoreExecutionResult ScriptExecutor::execute_from_string(const std::string& script_content, const sol::environment& env) const
    {
        return execute_lua([this, &script_content, &env]
        {
            return lua_.safe_script(script_content, env, sol::script_pass_on_error);
        });
    }
} // namespace engine::execution::script
