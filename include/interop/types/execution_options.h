/**
 * Project: Engine
 * File: execution_options.h
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
