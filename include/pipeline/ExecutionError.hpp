#pragma once

#include "pch.hpp"

namespace engine::pipeline
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
        ExecutionError(ErrorType type, std::string_view message)
            : std::runtime_error(std::string(message)), type_(type)
        {
        }

        ErrorType type() const noexcept
        {
            return type_;
        }

      private:
        ErrorType type_;
    };
} // namespace engine::pipeline