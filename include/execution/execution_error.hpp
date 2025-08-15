#pragma once

#include "execution_error_type.hpp"

namespace execution {
    class ExecutionError final : public std::runtime_error {
    public:
        explicit ExecutionError(ExecutionErrorType error_type,
                                std::string message)
            : std::runtime_error(std::move(message)), error_type_(error_type)
        {}

        ExecutionErrorType type() const noexcept { return error_type_; }

    private:
        ExecutionErrorType error_type_;
    };
} // namespace execution
