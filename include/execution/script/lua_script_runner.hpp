#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <sol/state.hpp>

#include "lua_state_manager.hpp"
#include "execution/core_execution_result.hpp"

namespace engine::execution::script
{
    struct ScriptContext
    {
        std::string script_content;
        std::optional<std::filesystem::path> input_path;
        std::optional<std::filesystem::path> output_path;
    };

    class LuaScriptRunner final
    {
    public:
        LuaScriptRunner(): lua_(LuaStateManager::get_state())
        {
        }

        CoreExecutionResult run_from_string(const ScriptContext& context) const;
        CoreExecutionResult run_from_file(const ScriptContext& context) const;

    private:
        sol::state& lua_;

        sol::environment create_script_env(const ScriptContext& context) const;
    };
} // namespace engine::execution::script
