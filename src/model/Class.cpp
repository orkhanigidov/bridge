#include "../../include/model/Class.h"

#include "../../include/pch.h"

namespace engine::model
{
    Class::Class(std::string id, const rttr::type& type) : m_id(std::move(id)), m_type(type)
    {
        if (m_id.empty())
            throw std::invalid_argument("Type ID cannot be empty");

        if (!m_type.is_valid())
            throw std::invalid_argument("Class type must be valid");
    }

    std::string Class::getId() const noexcept
    {
        return m_id;
    }

    rttr::type Class::getType() const noexcept
    {
        return m_type;
    }
} // namespace engine::model