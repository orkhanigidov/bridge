#include "conversion/oatpp_type_adapter.hpp"

namespace
{
    template <typename OatppType, typename CppType>
    engine::conversion::NativeVariant extract_or_default(const oatpp::Any& any)
    {
        if (any.get() == nullptr)
        {
            return CppType{};
        }

        const auto v = any.retrieve<typename OatppType::ObjectType>();
        return engine::conversion::NativeVariant{v.getValue(CppType{})};
    }
}

namespace engine::conversion
{
    using ConversionFunc = std::function<NativeVariant(const oatpp::Any&)>;

    NativeVariant OatppTypeAdapter::from_oatpp(const oatpp::Any& any)
    {
        if (!any)
        {
            return std::monostate{};
        }

        static const std::unordered_map<const oatpp::Type*, ConversionFunc> converter_map = {
            // Boolean
            {oatpp::Boolean::Class::getType(), extract_or_default<oatpp::Boolean, bool>},
            // Signed integers
            {oatpp::Int8::Class::getType(), extract_or_default<oatpp::Int8, std::int8_t>},
            {oatpp::Int16::Class::getType(), extract_or_default<oatpp::Int16, std::int16_t>},
            {oatpp::Int32::Class::getType(), extract_or_default<oatpp::Int32, std::int32_t>},
            {oatpp::Int64::Class::getType(), extract_or_default<oatpp::Int64, std::int64_t>},
            // Unsigned integers
            {oatpp::UInt8::Class::getType(), extract_or_default<oatpp::UInt8, std::uint8_t>},
            {oatpp::UInt16::Class::getType(), extract_or_default<oatpp::UInt16, std::uint16_t>},
            {oatpp::UInt32::Class::getType(), extract_or_default<oatpp::UInt32, std::uint32_t>},
            {oatpp::UInt64::Class::getType(), extract_or_default<oatpp::UInt64, std::uint64_t>},
            // Floating point
            {oatpp::Float32::Class::getType(), extract_or_default<oatpp::Float32, float>},
            {oatpp::Float64::Class::getType(), extract_or_default<oatpp::Float64, double>},
            // String
            {oatpp::String::Class::getType(), extract_or_default<oatpp::String, std::string>}
        };

        const auto* type = any.getStoredType();

        if (const auto it = converter_map.find(type); it != converter_map.end())
        {
            return it->second(any);
        }

        // Unsupported or complex types
        return std::monostate{};
    }
} // namespace engine::conversion
