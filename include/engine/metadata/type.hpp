#pragma once

#include "pch.hpp"

namespace engine::metadata
{
    enum class data_type : std::uint8_t
    {
        Void,
        Bool,
        Int,
        Float,
        Double,
        String,
        Vector,
        Map,
        Object,
        Custom
    };

    constexpr std::string_view to_string(data_type type) noexcept
    {
        constexpr std::array<std::string_view, 10> type_names = {
            "void",   "bool",   "int", "float",  "double",
            "string", "vector", "map", "object", "custom"};

        const auto index = static_cast<std::size_t>(type);
        return index < type_names.size() ? type_names[index] : "unknown";
    }

    constexpr data_type to_data_type(std::string_view type_str) noexcept
    {
        constexpr std::array<std::string_view, 9> type_names = {
            "void", "bool", "int", "float", "double", "string", "vector", "map", "object"};

        for (std::size_t i = 0; i < type_names.size(); ++i)
        {
            if (type_str == type_names[i])
            {
                return static_cast<data_type>(i);
            }
        }

        return data_type::Custom;
    }
} // namespace engine::metadata