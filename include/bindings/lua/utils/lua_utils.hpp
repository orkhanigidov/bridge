/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua::utils
{
    template <typename T>
    concept MappedContainer = requires
    {
        typename T::key_type;
        typename T::mapped_type;
    };

    template <typename Container>
        requires MappedContainer<Container>
    sol::table to_lua_table(sol::state_view lua, const Container& container)
    {
        sol::table table = lua.create_table_with(container.size(), 0);
        for (const auto& [key, value] : container)
        {
            table[key] = value;
        }
        return table;
    }

    template <typename Container>
        requires !MappedContainer<Container>
    sol::table to_lua_table(sol::state_view lua, const Container& container)
    {
        sol::table table = lua.create_table_with(0, container.size());
        for (const auto& elem : container)
        {
            table.add(elem);
        }
        return table;
    }
} // namespace engine::bindings::lua
