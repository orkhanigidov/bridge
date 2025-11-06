/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
