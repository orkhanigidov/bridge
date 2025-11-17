/**
 * Project: Engine
 * File: script_executor.hpp
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
 * @file script_executor.hpp
 * @brief Declares the ScriptExecutor utility for executing Lua scripts.
 */

#pragma once

#include <filesystem>
#include <string>
#include <sol/state.hpp>

#include "execution/core_execution_result.hpp"

namespace engine::execution::script
{
    /**
     * @class ScriptExecutor
     * @brief Utility class for executing Lua scripts from files or strings.
     */
    class ScriptExecutor final
    {
    public:
        /**
         * @brief Constructs a ScriptExecutor with the given Lua state.
         * @param lua The Lua state.
         */
        explicit ScriptExecutor(sol::state& lua) : lua_(lua)
        {
        }

        /**
         * @brief Executes a Lua script from a file.
         * @param script_path The path to the Lua script file.
         * @param env The Lua environment to use.
         * @return The result of the script execution.
         */
        CoreExecutionResult execute_from_file(const std::filesystem::path& script_path, const sol::environment& env) const;

        /**
         * @brief Executes a Lua script from a string.
         * @param script_content The Lua script content as a string.
         * @param env The Lua environment to use.
         * @return The result of the script execution.
         */
        CoreExecutionResult execute_from_string(const std::string& script_content, const sol::environment& env) const;

    private:
        /**
         * @brief Reference to the Lua state.
         */
        sol::state& lua_;
    };
} // namespace engine::execution::script
