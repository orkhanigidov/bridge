#include "../../include/operation/Parameter.h"
#include "../../include/pch.h"

namespace engine::operation
{
    Parameter::Parameter(int value): m_value(value)
    {
    }

    Parameter::Parameter(float value): m_value(value)
    {
    }

    Parameter::Parameter(double value): m_value(value)
    {
    }

    Parameter::Parameter(bool value): m_value(value)
    {
    }

    Parameter::Parameter(const std::string& value): m_value(value)
    {
    }

    Parameter::Parameter(const std::string& referenceId, const ParameterType type): m_type(type), m_value(referenceId)
    {
        if (type != ParameterType::Reference)
        {
            throw std::invalid_argument("Invalid parameter type for reference constructor");
        }
    }

    ParameterType Parameter::getType() const
    {
        return m_type;
    }

    bool Parameter::isReference() const
    {
        return m_type == ParameterType::Reference;
    }

    bool Parameter::isLiteral() const
    {
        return m_type == ParameterType::Literal;
    }

    bool Parameter::isInt() const
    {
        return isLiteral() && std::holds_alternative<int>(m_value);
    }

    bool Parameter::isFloat() const
    {
        return isLiteral() && std::holds_alternative<float>(m_value);
    }

    bool Parameter::isDouble() const
    {
        return isLiteral() && std::holds_alternative<double>(m_value);
    }

    bool Parameter::isBool() const
    {
        return isLiteral() && std::holds_alternative<bool>(m_value);
    }

    bool Parameter::isString() const
    {
        return isLiteral() && std::holds_alternative<std::string>(m_value);
    }

    int Parameter::asInt() const
    {
        return std::get<int>(m_value);
    }

    float Parameter::asFloat() const
    {
        return std::get<float>(m_value);
    }

    double Parameter::asDouble() const
    {
        return std::get<double>(m_value);
    }

    bool Parameter::asBool() const
    {
        return std::get<bool>(m_value);
    }

    std::string Parameter::asString() const
    {
        return std::get<std::string>(m_value);
    }

    std::string Parameter::getReference() const
    {
        if (!isReference()) { throw std::runtime_error("Parameter is not a reference"); }
        return std::get<std::string>(m_value);
    }

    const Parameter::ValueType& Parameter::getValue() const
    {
        return m_value;
    }

    Parameter Parameter::createReference(const std::string& referenceId)
    {
        return Parameter(referenceId, ParameterType::Reference);
    }
} // namespace engine::operation
