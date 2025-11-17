/**
 * Project: Engine
 * File: execution_request.h
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
 * @file execution_request.h
 * @brief Defines the ExecutionRequest struct for engine interop layer.
 */

#pragma once

#include "execution_options.h"
#include "execution_type.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @struct ExecutionRequest
     * @brief Represents a request to execute a script or pipeline in the engine interop layer.
     *
     * Contains the execution type, the script or file to execute, and execution options.
     */
    typedef struct ExecutionRequest
    {
        /**
         * @brief The type of execution (e.g., Lua, script, Lua file, pipeline).
         */
        ExecutionType type;

        /**
         * @brief The script or file to execute.
         */
        const char* script;

        /**
         * @brief Options for the execution, such as timeout and output format.
         */
        ExecutionOptions options;
    } ExecutionRequest;
} // namespace engine::interop::types

#pragma pack(pop)
