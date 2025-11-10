/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file oatpp_type_adapter.cpp
 * @brief Implements the OatppTypeAdapter utility for converting oatpp::Any to native C++ types.
 */

#include "network/serialization/oatpp_type_adapter.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <oatpp/core/Types.hpp>

namespace
{
    /**
     * @brief Extracts the value from oatpp::Any as a specific Oat++ type and converts it to a native C++ type.
     * @tparam OatppType The Oat++ type to extract.
     * @tparam CppType The native C++ type to convert to.
     * @param any The oatpp::Any value.
     * @return The extracted value as a NativeVariant.
     */
    template <typename OatppType, typename CppType>
    engine::network::serialization::NativeVariant extract_or_default(const oatpp::Any& any)
    {
        const auto v = any.retrieve<OatppType>();
        return engine::network::serialization::NativeVariant{v.getValue(CppType{})};
    }

    using ConversionFunc = std::function<engine::network::serialization::NativeVariant(const oatpp::Any&)>;

    /**
     * @brief Returns a static map of Oat++ types to their corresponding conversion functions.
     * @return Reference to the converter map.
     */
    const std::unordered_map<const oatpp::Type*, ConversionFunc>& get_converter_map()
    {
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
        return converter_map;
    }
}

namespace engine::network::serialization
{
    using ConversionFunc = std::function<NativeVariant(const oatpp::Any&)>;

    /**
     * @brief Converts an oatpp::Any value to a NativeVariant.
     * @param any The oatpp::Any value to convert.
     * @return The corresponding NativeVariant, or std::monostate if unsupported or null.
     */
    NativeVariant OatppTypeAdapter::from_oatpp(const oatpp::Any& any)
    {
        if (!any)
        {
            return std::monostate{};
        }

        const auto& converter_map = get_converter_map();
        const auto* type = any.getStoredType();

        if (const auto it = converter_map.find(type); it != converter_map.end())
        {
            return it->second(any);
        }

        // Unsupported or complex types
        return std::monostate{};
    }
} // namespace engine::network::serialization
