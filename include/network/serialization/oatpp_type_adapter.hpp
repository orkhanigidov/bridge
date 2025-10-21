#pragma once

#include <oatpp/core/Types.hpp>

namespace engine::network::serialization
{
    using NativeVariant = std::variant<
        std::monostate,
        bool,
        std::int8_t, std::uint8_t,
        std::int16_t, std::uint16_t,
        std::int32_t, std::uint32_t,
        std::int64_t, std::uint64_t,
        float,
        double,
        std::string
    >;

    class OatppTypeAdapter final
    {
    public:
        OatppTypeAdapter() = delete;

        static NativeVariant from_oatpp(const oatpp::Any& any);
    };
} // namespace engine::network::serialization
