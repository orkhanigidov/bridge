/**
 * Project: Engine
 * File: cpp_script_runner.hpp
 *
 * Copyright (C) 2026 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file cpp_script_runner.hpp
 * @brief Declares the CppScriptRunner utility for compiling and running C++ scripts.
 */

#pragma once

#include <string>
#include "execution/core_execution_result.hpp"
#include "execution/script/lua_script_runner.hpp"

namespace engine::execution::script
{
    /**
     * @class CppScriptRunner
     * @brief Utility for compiling and executing C++ layout examples.
     */
    class CppScriptRunner
    {
    public:
        /**
         * @brief Compiles and runs a C++ script from a given context.
         * @param context The execution context containing source and paths.
         * @return The result of the execution.
         */
        CoreExecutionResult run_from_string(const ScriptContext& context) const;
    };
} // namespace engine::execution::script
