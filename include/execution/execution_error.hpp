#pragma once

#include "interop/types/execution_error_type.h"

namespace engine::execution {

    class ExecutionError final : public std::runtime_error {
    public:
        explicit ExecutionError(interop::types::ExecutionErrorType error_type, const std::string& message)
            : std::runtime_error(message), m_error_type(error_type) {}

        interop::types::ExecutionErrorType error_type() const noexcept {
            return m_error_type;
        }

    private:
        interop::types::ExecutionErrorType m_error_type;
    };

} // namespace engine::execution
