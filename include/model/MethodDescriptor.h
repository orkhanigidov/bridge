#pragma once

#include "../pch.h"

using namespace rttr;

namespace engine::model
{

class MethodParameterDescriptor
{
  public:
    MethodParameterDescriptor() = delete;
    explicit MethodParameterDescriptor(const argument &name, const type &paramType, const variant &defaultValue)
        : m_name(name), m_paramType(paramType), m_defaultValue(defaultValue)
    {
    }

    const argument &getName() const
    {
        return m_name;
    }

    const type &getParamType() const
    {
        return m_paramType;
    }

    const variant &getDefaultValue() const
    {
        return m_defaultValue;
    }

  private:
    argument m_name;
    type m_paramType;
    variant m_defaultValue;
};

class MethodDescriptor
{
  public:
    MethodDescriptor() = delete;
    explicit MethodDescriptor(const method &method, const type &returnType,
                              const std::vector<MethodParameterDescriptor> &parameters, const std::string &category,
                              const std::string &description)
        : m_method(method), m_returnType(returnType), m_parameters(parameters), m_category(std::move(category)),
          m_description(std::move(description))
    {
    }

    const method &getMethod() const
    {
        return m_method;
    }

    const type &getReturnType() const
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
    method m_method;
    type m_returnType;
    std::vector<MethodParameterDescriptor> m_parameters;
    std::string m_category;
    std::string m_description;
};

} // namespace engine::model