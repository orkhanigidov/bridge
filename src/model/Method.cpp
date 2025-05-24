#include "../../include/model/Method.h"

#include "../../include/pch.h"

namespace engine::model
{
    Method::Method(std::string name, const rttr::type& returnType, std::vector<Parameter> parameters,
                   std::string category, std::string description, bool isStatic)
        : m_name(std::move(name)), m_returnType(returnType), m_parameters(std::move(parameters)),
          m_category(std::move(category)), m_description(std::move(description), m_isStatic(isStatic))
    {
        if (m_name.empty())
            throw std::invalid_argument("Method name cannot be empty");

        if (!m_returnType.is_valid())
            throw std::invalid_argument("Return type must be valid");
    }

    const std::string& Method::getName() const noexcept
    {
        return m_name;
    }

    const rttr::type& Method::getReturnType() const noexcept
    {
        return m_returnType;
    }

    const std::vector<Parameter>& Method::getParameters() const noexcept
    {
        return m_parameters;
    }

    const std::string& Method::getCategory() const noexcept
    {
        return m_category;
    }

    const std::string& Method::getDescription() const noexcept
    {
        return m_description;
    }

    bool Method::isStatic() const noexcept
    {
        return m_isStatic;
    }
} // namespace engine::model