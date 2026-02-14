/**
 * Project: Engine
 * File: execution_metadata.h
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
