#include "pipeline/ExecutionError.hpp"

#include "pch.hpp"

namespace engine::operation
{
    ExecutionError::ExecutionError(const ErrorType type, const std::string& message)
        : std::runtime_error(message), m_type(type)
    {
    }

    ErrorType ExecutionError::getType() const
    {
        return m_type;
    }
} // namespace engine::operation
