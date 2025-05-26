#include "../../include/model/Method.h"

#include "../../include/pch.h"

#include <utility>

namespace engine::model
{
    Method::Method(const rttr::method method, const rttr::type& returnType, const std::vector<Parameter>& parameters,
                   std::string category, std::string description, const bool isStatic)
        : m_method(method), m_returnType(returnType), m_parameters(parameters), m_category(std::move(category)),
          m_description(std::move(description)), m_isStatic(isStatic)
    {
        if (!method.is_valid())
            throw std::invalid_argument("Method must be valid");

        if (!m_returnType.is_valid())
            throw std::invalid_argument("Return type must be valid");
    }

    rttr::method Method::getMethod() const noexcept
    {
        return m_method;
    }

    rttr::type Method::getReturnType() const noexcept
    {
        return m_returnType;
    }

    const std::vector<Parameter>& Method::getParameters() const noexcept
    {
        return m_parameters;
    }

    std::string Method::getCategory() const noexcept
    {
        return m_category;
    }

    std::string Method::getDescription() const noexcept
    {
        return m_description;
    }

    bool Method::isStatic() const noexcept
    {
        return m_isStatic;
    }
} // namespace engine::model