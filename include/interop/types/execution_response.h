/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
