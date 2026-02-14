/**
 * Project: Engine
 * File: execution_error_type.h
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
 * @file execution_error_type.h
 * @brief Defines the ExecutionErrorType enum for engine interop layer.
 */

#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @enum ExecutionErrorType
     * @brief Represents error types that can occur during execution in the engine interop layer.
     *
     * Used to indicate specific error conditions such as invalid arguments, execution failure,
     * validation failure, or file not found.
     */
    typedef enum ExecutionErrorType
    {
        /**
         * @brief The provided argument is invalid.
         */
        InvalidArgument = 0,

        /**
         * @brief The execution failed.
         */
        ExecutionFailed = 1,

        /**
         * @brief The validation failed.
         */
        ValidationFailed = 2,

        /**
         * @brief The required file was not found.
         */
        FileNotFound = 3
    } ExecutionErrorType;
} // namespace engine::interop::types

#pragma pack(pop)
