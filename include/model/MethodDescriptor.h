#pragma once

#include "../pch.h"

#include "MethodParameterDescriptor.h"

namespace engine::model
{

class MethodDescriptor
{
  public:
    MethodDescriptor() = delete;
    explicit MethodDescriptor(const rttr::method &method, const rttr::type &returnType,
                              const std::vector<MethodParameterDescriptor> &parameters, const std::string &category,
                              const std::string &description)
        : m_method(method), m_returnType(returnType), m_parameters(parameters), m_category(std::move(category)),
          m_description(std::move(description))
    {
    }

    const rttr::method &getMethod() const
    {
        return m_method;
    }

    const rttr::type &getReturnType() const
    {
        return m_returnType;
    }

    const std::vector<MethodParameterDescriptor> &getParameters() const
    {
        return m_parameters;
    }

    const std::string &getCategory() const
    {
        return m_category;
    }

    const std::string &getDescription() const
    {
        return m_description;
    }

  private:
    rttr::method m_method;
    rttr::type m_returnType;
    std::vector<MethodParameterDescriptor> m_parameters;
    std::string m_category;
    std::string m_description;
};

} // namespace engine::model