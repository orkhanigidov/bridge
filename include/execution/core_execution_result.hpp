/**
 * Project: Engine
 * File: core_execution_result.hpp
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
 * @file core_execution_result.hpp
 * @brief Defines core execution result types and data structures for the engine.
 */

#pragma once

#include <string>

namespace engine::execution
{
    /**
     * @enum CoreExecutionStatus
     * @brief Represents the status of core execution.
     */
    enum class CoreExecutionStatus
    {
        /**
         * @brief Execution completed successfully.
         */
        Success,

        /**
         * @brief Execution failed.
         */
        Failure,

        /**
         * @brief Execution timed out.
         */
        Timeout
    };

    /**
     * @enum CoreExecutionErrorType
     * @brief Enumerates possible error types for core execution.
     */
    enum class CoreExecutionErrorType
    {
        /**
         * @brief Invalid argument was provided.
         */
        InvalidArgument,

        /**
         * @brief Execution failed due to an internal error.
         */
        ExecutionFailed,

        /**
         * @brief Validation of input or output failed.
         */
        ValidationFailed,

        /**
         * @brief The required file was not found.
         */
        FileNotFound
    };

    /**
     * @struct CoreExecutionError
     * @brief Contains an error type and message for a failed execution.
     */
    struct CoreExecutionError
    {
        /**
         * @brief Type of the error.
         */
        CoreExecutionErrorType type = CoreExecutionErrorType::ExecutionFailed;

        /**
         * @brief Detailed error message.
         */
        std::string message;
    };

    /**
     * @struct CoreExecutionMetadata
     * @brief Stores metadata about the execution, such as duration.
     */
    struct CoreExecutionMetadata
    {
        /**
         * @brief Duration of execution in milliseconds.
         */
        long long duration_milliseconds = 0;
    };

    /**
     * @struct CoreExecutionResult
     * @brief Represents the result of a core execution, including status, error, metadata, and output.
     */
    struct CoreExecutionResult
    {
        /**
         * @brief Status of the execution.
         */
        CoreExecutionStatus status = CoreExecutionStatus::Failure;

        /**
         * @brief Error information if execution failed.
         */
        CoreExecutionError error;

        /**
         * @brief Metadata about the execution.
         */
        CoreExecutionMetadata metadata;

        /**
         * @brief Output data produced by the execution.
         */
        std::string output_data;

        /**
         * @brief Optional string output from the script.
         */
        std::string string_output;

        /**
         * @brief Checks if the execution was successful.
         * @return True if status is Success, false otherwise.
         */
        bool is_success() const
        {
            return status == CoreExecutionStatus::Success;
        }
    };
}
