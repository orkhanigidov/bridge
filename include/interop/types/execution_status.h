/**
 * Project: Engine
 * File: execution_status.h
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
 * @file execution_status.h
 * @brief Defines the ExecutionStatus enum for the engine interop layer.
 */

#pragma once

#pragma pack(push, 8)

namespace engine::interop::types
{
    /**
     * @enum ExecutionStatus
     * @brief Represents the status of an execution in the engine interop layer.
     *
     * Used to indicate whether an operation was successful, failed, or timed out.
     */
    typedef enum ExecutionStatus
    {
        /**
         * @brief The operation completed successfully.
         */
        Success = 0,

        /**
         * @brief The operation failed.
         */
        Failure = 1,

        /**
         * @brief The operation timed out.
         */
        Timeout = 2
    } ExecutionStatus;
} // namespace engine::interop::types

#pragma pack(pop)
