#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua::utils
{
    template <typename Container>
    sol::table to_lua_table(sol::state& lua, const Container& container)
    {
        sol::table table = lua.create_table_with(container.size(), 0);

        for (const auto& elem : container)
        {
            table.add(elem);
        }
        return table;
    }
} // namespace engine::bindings::lua
