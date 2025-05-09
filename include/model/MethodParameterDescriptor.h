#pragma once

#include "../pch.h"

namespace engine::model
{

class MethodParameterDescriptor
{
  public:
    MethodParameterDescriptor() = delete;
    explicit MethodParameterDescriptor(const rttr::argument &name, const rttr::type &paramType,
                                       const rttr::variant &defaultValue)
        : m_name(name), m_paramType(paramType), m_defaultValue(defaultValue)
    {
    }

    const rttr::argument &getName() const
    {
        return m_name;
    }

    const rttr::type &getParamType() const
    {
        return m_paramType;
    }

    const rttr::variant &getDefaultValue() const
    {
        return m_defaultValue;
    }

  private:
    rttr::argument m_name;
    rttr::type m_paramType;
    rttr::variant m_defaultValue;
};

} // namespace engine::model