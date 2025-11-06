/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
