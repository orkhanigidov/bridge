/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_options.h
 * @brief Defines the ExecutionOptions struct for the engine interop layer.
 */

#pragma once

#include <cstdint>

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @struct ExecutionOptions
     * @brief Represents options for execution in the engine interop layer.
     *
     * Contains configuration such as timeout and output data format.
     */
    typedef struct ExecutionOptions
    {
        /**
         * @brief Timeout for execution in milliseconds.
         */
        uint32_t timeout_milliseconds;

        /**
         * @brief Output data format (e.g., "graphml").
         */
        const char* output_data_format;
    } ExecutionOptions;
} // namespace engine::interop::types

#pragma pack(pop)
