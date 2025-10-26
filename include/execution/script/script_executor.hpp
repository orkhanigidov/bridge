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
        explicit ScriptExecutor(sol::state& lua): lua_(lua)
        {
        }

        CoreExecutionResult execute_from_file(const std::filesystem::path& script_path, const sol::environment& env) const;
        CoreExecutionResult execute_from_string(const std::string& script_content, const sol::environment& env) const;

    private:
        sol::state& lua_;
    };
} // namespace engine::execution::script
