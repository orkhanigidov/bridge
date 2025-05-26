#include "../../include/operation/Result.h"

#include "../../include/pch.h"

namespace engine::operation
{
    Result::Result(const rttr::variant& variant) : m_variant(variant), m_hasVariant(variant.is_valid()) {}

    rttr::variant Result::getVariant() const
    {
        if (!hasVariant())
            throw std::runtime_error("Result has no value");

        return m_variant;
    }

    bool Result::hasVariant() const
    {
        if (!m_hasVariant)
            throw std::runtime_error("Result has no value");

        return m_hasVariant;
    }
} // namespace engine::operation