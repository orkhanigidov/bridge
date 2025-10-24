#pragma once

#include <filesystem>
#include <string>
#include <sol/state.hpp>

#include "execution/core_execution_result.hpp"

namespace engine::execution::script
{
    class ScriptExecutor final
    {
    public:
        ScriptExecutor();

        CoreExecutionResult execute_from_file(const std::filesystem::path& script_path);
        CoreExecutionResult execute_from_string(const std::string& script_content);

    private:
        sol::state lua_;
    };
} // namespace engine::execution::script
