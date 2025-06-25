#pragma once

#include "oatpp/core/Types.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    using Value = std::variant<std::monostate, bool, std::int8_t, std::uint8_t, std::int16_t,
                               std::uint16_t, std::int32_t, std::uint32_t, std::int64_t,
                               std::uint64_t, float, double, std::string>;

    class Converter
    {
      public:
        static Value to_cpp_value(const oatpp::Any& any);
    };
} // namespace engine::serialization