/**
 * Project: Engine
 * File: lua_script_runner.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file lua_script_runner.hpp
 * @brief Declares the LuaScriptRunner utility for running Lua scripts with a context.
 */

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <sol/state.hpp>

#include "lua_state_manager.hpp"
#include "execution/core_execution_result.hpp"

namespace engine::execution::script
{
    /**
     * @struct ScriptContext
     * @brief Holds context information for running a Lua script.
     */
    struct ScriptContext
    {
        /**
         * @brief The Lua script content as a string.
         */
        std::string script_content;

        /**
         * @brief Optional input file path.
         */
        std::optional<std::filesystem::path> input_path;

        /**
         * @brief Optional output file path.
         */
        std::optional<std::filesystem::path> output_path;
    };

    /**
     * @class LuaScriptRunner
     * @brief Utility class for running Lua scripts from strings or files with a given context.
     */
    class LuaScriptRunner final
    {
    public:
        /**
         * @brief Constructs a LuaScriptRunner using the thread-local Lua state.
         */
        explicit LuaScriptRunner(sol::state& lua) : lua_(lua)
        {
        }

        /**
         * @brief Runs a Lua script from a string using the provided context.
         * @param context The script context containing script content and optional paths.
         * @return The result of the script execution.
         */
        CoreExecutionResult run_from_string(const ScriptContext& context) const;

        /**
         * @brief Runs a Lua script from a file using the provided context.
         * @param context The script context containing script content and optional paths.
         * @return The result of the script execution.
         */
        CoreExecutionResult run_from_file(const ScriptContext& context) const;

    private:
        /**
         * @brief Reference to the thread-local Lua state.
         */
        sol::state& lua_;

        /**
         * @brief Creates a Lua environment for the script using the provided context.
         * @param context The script context.
         * @return The created Lua environment.
         */
        sol::environment create_script_env(const ScriptContext& context) const;
    };
} // namespace engine::execution::script
