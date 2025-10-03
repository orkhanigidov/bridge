#include "conversion/oatpp_type_adapter.hpp"

namespace {
    
    template<typename OatppType, typename CppType>
    engine::conversion::NativeVariant extract_or_default(const oatpp::Any& any, const CppType& default_value) {
        const auto v = any.retrieve<OatppType>();
        return v ? engine::conversion::NativeVariant{v.getValue(default_value)} : std::monostate{};
    }

}

namespace engine::conversion {

    NativeVariant OatppTypeAdapter::from_oatpp(const oatpp::Any& any)
    {
        if (!any) {
            return std::monostate{};
        }

        const auto* type = any.getStoredType();

        // Boolean
        if (type == oatpp::Boolean::Class::getType()) {
            return extract_or_default<oatpp::Boolean, bool>(any, false);
        }

        // Signed integers
        if (type == oatpp::Int8::Class::getType()) {
            return extract_or_default<oatpp::Int8, std::int8_t>(any, 0);
        }
        if (type == oatpp::Int16::Class::getType()) {
            return extract_or_default<oatpp::Int16, std::int16_t>(any, 0);
        }
        if (type == oatpp::Int32::Class::getType()) {
            return extract_or_default<oatpp::Int32, std::int32_t>(any, 0);
        }
        if (type == oatpp::Int64::Class::getType()) {
            return extract_or_default<oatpp::Int64, std::int64_t>(any, 0);
        }

        // Unsigned integers
        if (type == oatpp::UInt8::Class::getType()) {
            return extract_or_default<oatpp::UInt8, std::uint8_t>(any, 0);
        }
        if (type == oatpp::UInt16::Class::getType()) {
            return extract_or_default<oatpp::UInt16, std::uint16_t>(any, 0);
        }
        if (type == oatpp::UInt32::Class::getType()) {
            return extract_or_default<oatpp::UInt32, std::uint32_t>(any, 0);
        }
        if (type == oatpp::UInt64::Class::getType()) {
            return extract_or_default<oatpp::UInt64, std::uint64_t>(any, 0);
        }

        // Floating point
        if (type == oatpp::Float32::Class::getType()) {
            return extract_or_default<oatpp::Float32, float>(any, 0.0f);
        }
        if (type == oatpp::Float64::Class::getType()) {
            return extract_or_default<oatpp::Float64, double>(any, 0.0);
        }

        // String
        if (type == oatpp::String::Class::getType()) {
            return extract_or_default<oatpp::String, std::string>(any, "");
        }

        // Unsupported or complex types
        return std::monostate{};
    }

} // namespace engine::conversion
