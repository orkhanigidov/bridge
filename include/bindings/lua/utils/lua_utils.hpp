#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua::utils
{
    template <typename T, typename = void>
    struct is_mapped_type : std::false_type
    {
    };

    template <typename T>
    struct is_mapped_type<T, std::void_t<typename T::key_type, typename T::mapped_type>> : std::true_type
    {
    };

    template <typename Container, std::enable_if_t<!is_mapped_type<Container>::value, int>  = 0>
    sol::table to_lua_table(sol::state_view lua, const Container& container)
    {
        sol::table table = lua.create_table_with(container.size(), 0);

        for (const auto& elem : container)
        {
            table.add(elem);
        }
        return table;
    }

    template <typename Container, std::enable_if_t<is_mapped_type<Container>::value, int>  = 0>
    sol::table to_lua_table(sol::state_view lua, const Container& container)
    {
        sol::table table = lua.create_table_with(0, container.size());

        for (const auto& [key, value] : container)
        {
            table[key] = value;
        }
        return table;
    }
} // namespace engine::bindings::lua
