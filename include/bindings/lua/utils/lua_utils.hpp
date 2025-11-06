/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file lua_utils.hpp
 * @brief Utility functions for converting C++ containers to Lua tables.
 */

#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua::utils
{
    /**
     * @concept MappedContainer
     * @brief Concept for containers with key-value poirs (e.g., std::map).
     */
    template <typename T>
    concept MappedContainer = requires
    {
        typename T::key_type;
        typename T::mapped_type;
    };

    /**
     * @brief Converts a mapped container (e.g., std::map) to a Lua table.
     * @tparam Container The container type (must satisfy MappedContainer).
     * @param lua The Lua state view.
     * @param container The container to convert.
     * @return A Lua table with key-value pairs.
     */
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

    /**
     * @brief Converts a sequential container (e.g., std::vector) to a Lua table.
     * @tparam Container The container type (must not satisfy MappedContainer).
     * @param lua The Lua state view.
     * @param container The container to convert.
     * @return A Lua table with sequential elements.
     */
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
