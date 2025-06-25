#include "serialization/Converter.hpp"

#include "pch.hpp"

namespace engine::serialization
{
    Value Converter::to_cpp_value(const oatpp::Any& any)
    {
        if (!any)
        {
            return std::monostate{};
        }

        if (any.getStoredType() == oatpp::String::Class::getType())
            return any.retrieve<oatpp::String>();
        if (any.getStoredType() == oatpp::Int8::Class::getType())
            return any.retrieve<oatpp::Int8>();
        if (any.getStoredType() == oatpp::UInt8::Class::getType())
            return any.retrieve<oatpp::UInt8>();
        if (any.getStoredType() == oatpp::Int16::Class::getType())
            return any.retrieve<oatpp::Int16>();
        if (any.getStoredType() == oatpp::UInt16::Class::getType())
            return any.retrieve<oatpp::UInt16>();
        if (any.getStoredType() == oatpp::Int32::Class::getType())
            return any.retrieve<oatpp::Int32>();
        if (any.getStoredType() == oatpp::UInt32::Class::getType())
            return any.retrieve<oatpp::UInt32>();
        if (any.getStoredType() == oatpp::Int64::Class::getType())
            return any.retrieve<oatpp::Int64>();
        if (any.getStoredType() == oatpp::UInt64::Class::getType())
            return any.retrieve<oatpp::UInt64>();
        if (any.getStoredType() == oatpp::Float32::Class::getType())
            return any.retrieve<oatpp::Float32>();
        if (any.getStoredType() == oatpp::Float64::Class::getType())
            return any.retrieve<oatpp::Float64>();
        if (any.getStoredType() == oatpp::Boolean::Class::getType())
            return any.retrieve<oatpp::Boolean>();

        return std::monostate{};
    }
} // namespace engine::serialization
