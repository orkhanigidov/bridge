/**
 * Project: Engine
 * File: execution_response.h
 *
 * Copyright (C) 2025 Orkhan Igidov
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
 * @file execution_response.h
 * @brief Defines the ExecutionResponse struct for engine interop layer.
 */

#pragma once

#include "execution_error.h"
#include "execution_metadata.h"
#include "execution_status.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @struct ExecutionResponse
     * @brief Represents the response of an execution in the engine interop layer.
     *
     * Contains the execution status, error information, and execution metadata.
     */
    typedef struct ExecutionResponse
    {
        /**
         * @brief The status of the execution (e.g., success, failure).
         */
        ExecutionStatus status;

        /**
         * @brief Error information if the execution failed.
         */
        ExecutionError error;

        /**
         * @brief Optional string output from the script.
         */
        char* string_output;

        /**
         * @brief Metadata about the execution, such as duration.
         */
        ExecutionMetadata metadata;
    } ExecutionResponse;
} // namespace engine::interop::types

#pragma pack(pop)
