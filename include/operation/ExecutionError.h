#pragma once

#include "../pch.h"

namespace engine::operation
{
    enum class ErrorType
    {
        InvalidArgument,
        InvalidFormat,
        InvalidReference,
        InvalidConversion,
        MissingNode,
        MissingArgument,
        MissingReference,
        MissingResult,
        FailedExecution,
        FailedConversion,
        FailedValidation,
        CyclicDependency
    };

    class ExecutionError final : public std::runtime_error
    {
    public:
        ExecutionError(ErrorType type, const std::string& message);

        ErrorType getType() const;

    private:
        ErrorType m_type;
    };
} // namespace engine::operation
