#include "../../include/model/Parameter.h"

#include "../../include/pch.h"

namespace engine::model
{
    Parameter::Parameter(std::string name, const rttr::type& type, rttr::variant defaultValue)
        : m_name(std::move(name)), m_type(type), m_defaultValue(std::move(defaultValue))
    {
        if (m_name.empty())
            throw std::invalid_argument("Parameter name cannot be empty");

        if (!m_type.is_valid())
            throw std::invalid_argument("Parameter type must be valid");
    }

    const std::string& Parameter::getName() const noexcept
    {
        return m_name;
    }

    const rttr::type& Parameter::getType() const noexcept
    {
        return m_type;
    }

    const rttr::variant& Parameter::getDefaultValue() const noexcept
    {
        return m_defaultValue;
    }
} // namespace engine::model