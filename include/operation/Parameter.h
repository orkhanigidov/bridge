#pragma once

#include "../pch.h"

namespace engine::operation
{
    enum class ParameterType { None, Literal, Reference };

    class Parameter
    {
    public:
        using ValueType = std::variant<int, float, double, bool, std::string>;

        explicit Parameter(int value);
        explicit Parameter(float value);
        explicit Parameter(double value);
        explicit Parameter(bool value);
        explicit Parameter(const std::string& value);
        Parameter(const std::string& referenceId, ParameterType type);

        Parameter(): m_type(ParameterType::None)
        {
        }

        ~Parameter() = default;

        ParameterType getType() const;
        bool isReference() const;
        bool isLiteral() const;

        bool isInt() const;
        bool isFloat() const;
        bool isDouble() const;
        bool isBool() const;
        bool isString() const;

        int asInt() const;
        float asFloat() const;
        double asDouble() const;
        bool asBool() const;
        std::string asString() const;

        const ValueType& getValue() const;
        std::string getReference() const;

        static Parameter createReference(const std::string& referenceId);

    private:
        ParameterType m_type{ParameterType::Literal};
        ValueType m_value;
    };
} // namespace engine::operation
