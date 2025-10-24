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
        Invalid_Argument,
        Execution_Failed,
        Validation_Failed,
        File_Not_Found
    };

    struct CoreExecutionError
    {
        CoreExecutionErrorType type;
        std::string message;
    };

    struct CoreExecutionMetadata
    {
        long long duration_milliseconds;
    };

    struct CoreExecutionResult
    {
        CoreExecutionStatus status;
        CoreExecutionError error;
        CoreExecutionMetadata metadata;

        std::string output_data;

        bool is_success() const
        {
            return status == CoreExecutionStatus::Success;
        }
    };
}
