#pragma once

#include <oatpp/core/Types.hpp>

namespace serialization {
    using ConvertedValue
            = std::variant<std::monostate, bool, std::int8_t, std::uint8_t,
                           std::int16_t, std::uint16_t, std::int32_t,
                           std::uint32_t, std::int64_t, std::uint64_t, float,
                           double, std::string>;

    class OatppTypeAdapter final {
    public:
        static ConvertedValue from_oatpp(const oatpp::Any& value);
    };
} // namespace serialization
