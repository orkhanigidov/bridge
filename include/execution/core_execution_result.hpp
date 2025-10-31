/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <string>

namespace engine::execution
{
    enum class CoreExecutionStatus
    {
        Success,
        Failure,
        Timeout
    };

    enum class CoreExecutionErrorType
    {
        InvalidArgument,
        ExecutionFailed,
        ValidationFailed,
        FileNotFound
    };

    struct CoreExecutionError
    {
        CoreExecutionErrorType type = CoreExecutionErrorType::ExecutionFailed;
        std::string message;
    };

    struct CoreExecutionMetadata
    {
        long long duration_milliseconds = 0;
    };

    struct CoreExecutionResult
    {
        CoreExecutionStatus status = CoreExecutionStatus::Failure;
        CoreExecutionError error;
        CoreExecutionMetadata metadata;

        std::string output_data;

        bool is_success() const
        {
            return status == CoreExecutionStatus::Success;
        }
    };
}
