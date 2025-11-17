/**
 * Project: Engine
 * File: execution_engine.hpp
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
 * @file execution_engine.hpp
 * @brief Declares the ExecutionEngine utility for executing scripts.
 */

#pragma once

#include <string_view>

#include "interop/types/execution_type.h"
#include "utils/response_factory.hpp"

namespace engine::execution
{
    /**
     * @class ExecutionEngine
     * @brief Utility class for executing scripts based on the execution type.
     */
    class ExecutionEngine final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ExecutionEngine() = delete;

        /**
         * @brief Executes a script based on the specified execution type.
         * @param type The execution type.
         * @param script The script to execute.
         * @return A pointer to the execution response.
         */
        static utils::ExecutionResponsePtr execute(interop::types::ExecutionType type, std::string_view script);
    };
} // namespace engine::execution
