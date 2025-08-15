#include "serialization/oatpp_type_adapter.hpp"

namespace serialization {
    ConvertedValue OatppTypeAdapter::from_oatpp(const oatpp::Any& value)
    {
        if (!value) { return std::monostate{}; }

        const auto& type = value.getStoredType();

        // String
        if (type == oatpp::String::Class::getType()) {
            const auto v = value.retrieve<oatpp::String>();
            return v ? v.getValue("") : std::monostate{};
        }

        // Signed integers
        if (type == oatpp::Int8::Class::getType()) {
            const auto v = value.retrieve<oatpp::Int8>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::Int16::Class::getType()) {
            const auto v = value.retrieve<oatpp::Int16>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::Int32::Class::getType()) {
            const auto v = value.retrieve<oatpp::Int32>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::Int64::Class::getType()) {
            const auto v = value.retrieve<oatpp::Int64>();
            return v ? v.getValue(0) : std::monostate{};
        }

        // Unsigned integers
        if (type == oatpp::UInt8::Class::getType()) {
            const auto v = value.retrieve<oatpp::UInt8>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::UInt16::Class::getType()) {
            const auto v = value.retrieve<oatpp::UInt16>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::UInt32::Class::getType()) {
            const auto v = value.retrieve<oatpp::UInt32>();
            return v ? v.getValue(0) : std::monostate{};
        }
        if (type == oatpp::UInt64::Class::getType()) {
            const auto v = value.retrieve<oatpp::UInt64>();
            return v ? v.getValue(0) : std::monostate{};
        }

        // Floating point
        if (type == oatpp::Float32::Class::getType()) {
            const auto v = value.retrieve<oatpp::Float32>();
            return v ? v.getValue(0.0f) : std::monostate{};
        }
        if (type == oatpp::Float64::Class::getType()) {
            const auto v = value.retrieve<oatpp::Float64>();
            return v ? v.getValue(0.0) : std::monostate{};
        }

        // Boolean
        if (type == oatpp::Boolean::Class::getType()) {
            const auto v = value.retrieve<oatpp::Boolean>();
            return v ? v.getValue(false) : std::monostate{};
        }

        // Unsupported or complex types
        return std::monostate{};
    }
} // namespace serialization
