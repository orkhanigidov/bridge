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
    CoreExecutionResult ScriptExecutor::execute_from_file(const std::filesystem::path& script_path) const
    {
        const auto normalized_path = utils::filesystem::to_forward_slashes(std::filesystem::absolute(script_path));

        return execute_lua([&]
        {
            return lua_.safe_script_file(normalized_path);
        });
    }

    CoreExecutionResult ScriptExecutor::execute_from_string(const std::string& script_content) const
    {
        return execute_lua([&]
        {
            return lua_.safe_script(script_content);
        });
    }
} // namespace engine::execution::script
