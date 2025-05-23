#include "../../include/pch.h"
#include "../../include/operation/Result.h"

namespace engine::operation
{
    Result::Result(const rttr::variant& value): m_value(value), m_hasValue(value.is_valid())
    {
    }

    const rttr::variant& Result::getValue() const
    {
        return m_value;
    }

    bool Result::hasValue() const
    {
        return m_hasValue && m_value.is_valid();
    }
} // namespace engine::operation
