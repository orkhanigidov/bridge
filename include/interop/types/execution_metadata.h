/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_metadata.h
 * @brief Defines the ExecutionMetadata struct for engine interop layer.
 */

#pragma once

#include <cstdint>

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @struct ExecutionMetadata
     * @brief Represents metadata for an execution in the engine interop layer.
     *
     * Contains information such as the duration of the execution in milliseconds.
     */
    typedef struct ExecutionMetadata
    {
        /**
         * @brief Duration of the execution in milliseconds.
         */
        uint64_t duration_milliseconds;
    } ExecutionMetadata;
} // namespace engine::interop::types

#pragma pack(pop)
