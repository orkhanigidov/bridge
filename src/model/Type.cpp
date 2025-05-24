#include "../../include/model/Type.h"

#include "../../include/pch.h"

namespace engine::model
{
    Type::Type(std::string id, std::string name) : m_id(std::move(id)), m_name(std::move(name))
    {
        if (m_id.empty())
            throw std::invalid_argument("Type ID cannot be empty");

        if (m_name.empty())
            throw std::invalid_argument("Type name cannot be empty");
    }

    const std::string& Type::getId() const noexcept
    {
        return m_id;
    }

    const std::string& Type::getName() const noexcept
    {
        return m_name;
    }
} // namespace engine::model