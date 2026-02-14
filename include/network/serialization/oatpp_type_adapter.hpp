/**
 * Project: Engine
 * File: oatpp_type_adapter.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file oatpp_type_adapter.hpp
 * @brief Defines the OatppTypeAdapter utility for converting oatpp::Any to native C++ types.
 */

#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <oatpp/core/Types.hpp>

namespace engine::network::serialization
{
    /**
     * @typedef NativeVariant
     * @brief Variant type representing supported native C++ types for conversion.
     */
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

    /**
     * @class OatppTypeAdapter
     * @brief Utility class for converting oatpp::Any to NativeVariant.
     *
     * Provides a static method to convert oatpp::Any values to their corresponding native C++ types.
     */
    class OatppTypeAdapter final
    {
    public:
        OatppTypeAdapter() = delete;

        /**
         * @brief Converts an oatpp::Any value to a NativeVariant.
         * @param any The oatpp::Any value to convert.
         * @return The corresponding NativeVariant.
         */
        static NativeVariant from_oatpp(const oatpp::Any& any);
    };
} // namespace engine::network::serialization
