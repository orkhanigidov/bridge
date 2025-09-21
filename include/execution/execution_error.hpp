#pragma once

#include "execution_error_type.hpp"

namespace execution {

    class ExecutionError final : public std::runtime_error {
    public:
        explicit ExecutionError(ExecutionErrorType error_type, const std::string& message)
            : std::runtime_error(message), m_error_type(error_type) {}

        ExecutionErrorType error_type() const noexcept {
            return m_error_type;
        }

    private:
        ExecutionErrorType m_error_type;
    };

} // namespace execution
