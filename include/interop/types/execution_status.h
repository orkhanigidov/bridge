/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
