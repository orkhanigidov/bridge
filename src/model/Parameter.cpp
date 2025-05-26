#include "../../include/model/Parameter.h"

#include "../../include/pch.h"

namespace engine::model
{
    Parameter::Parameter(std::string name, const rttr::type& type, rttr::variant defaultValue, const bool isReference)
        : m_name(std::move(name)), m_type(type), m_defaultValue(std::move(defaultValue)), m_isReference(isReference)
    {
        if (m_name.empty())
            throw std::invalid_argument("Parameter name cannot be empty");

        if (!m_type.is_valid())
            throw std::invalid_argument("Parameter type must be valid");
    }

    std::string Parameter::getName() const noexcept
    {
        return m_name;
    }

    rttr::type Parameter::getType() const noexcept
    {
        return m_type;
    }

    rttr::variant Parameter::getDefaultValue() const noexcept
    {
        return m_defaultValue;
    }

    bool Parameter::isReference() const noexcept
    {
        return m_isReference;
    }
} // namespace engine::model