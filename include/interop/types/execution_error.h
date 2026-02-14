/**
 * Project: Engine
 * File: execution_error.h
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
 * @file execution_error.h
 * @brief Defines the ExecutionError struct for engine interop layer.
 */

#pragma once

#include "execution_error_type.h"

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @struct ExecutionError
     * @brief Represents an error that occurred during execution in the engine interop layer.
     *
     * Contains the error type and an associated error message.
     */
    typedef struct ExecutionError
    {
        /**
         * @brief The type of the execution error.
         */
        ExecutionErrorType type;

        /**
         * @brief The error message describing the error.
         */
        char* message;
    } ExecutionError;
} // namespace engine::interop::types

#pragma pack(pop)
