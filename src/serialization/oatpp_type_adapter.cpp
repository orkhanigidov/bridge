#include "engine/serialization/oatpp_type_adapter.hpp"

#include "pch.hpp"

namespace engine::serialization
{
    converted_value oatpp_type_adapter::from_oatpp(const oatpp::Any& oatpp_value)
    {
        if (!oatpp_value)
        {
            return std::monostate{};
        }

        const auto& stored_type = oatpp_value.getStoredType();

        if (stored_type == oatpp::String::Class::getType())
            return oatpp_value.retrieve<oatpp::String>();
        if (stored_type == oatpp::Int8::Class::getType())
            return oatpp_value.retrieve<oatpp::Int8>();
        if (stored_type == oatpp::UInt8::Class::getType())
            return oatpp_value.retrieve<oatpp::UInt8>();
        if (stored_type == oatpp::Int16::Class::getType())
            return oatpp_value.retrieve<oatpp::Int16>();
        if (stored_type == oatpp::UInt16::Class::getType())
            return oatpp_value.retrieve<oatpp::UInt16>();
        if (stored_type == oatpp::Int32::Class::getType())
            return oatpp_value.retrieve<oatpp::Int32>();
        if (stored_type == oatpp::UInt32::Class::getType())
            return oatpp_value.retrieve<oatpp::UInt32>();
        if (stored_type == oatpp::Int64::Class::getType())
            return oatpp_value.retrieve<oatpp::Int64>();
        if (stored_type == oatpp::UInt64::Class::getType())
            return oatpp_value.retrieve<oatpp::UInt64>();
        if (stored_type == oatpp::Float32::Class::getType())
            return oatpp_value.retrieve<oatpp::Float32>();
        if (stored_type == oatpp::Float64::Class::getType())
            return oatpp_value.retrieve<oatpp::Float64>();
        if (stored_type == oatpp::Boolean::Class::getType())
            return oatpp_value.retrieve<oatpp::Boolean>();

        return std::monostate{};
    }
} // namespace engine::serialization
