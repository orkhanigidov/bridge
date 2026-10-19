/**
 * Project: Engine
 * File: lua_state_manager.cpp
 *
 * Copyright (C) 2025-2026 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "execution/script/lua_state_manager.hpp"

#include <memory>
#include <sol/state.hpp>

#include "bindings/lua/lua_binder.hpp"

namespace engine::execution::script
{
    /**
     * @brief Initializes the thread-local Lua state with restricted libraries and custom bindings.
     * @return The initialized Lua state.
     */
    std::unique_ptr<sol::state> LuaStateManager::initialize_thread_state()
    {
        auto lua_state = std::make_unique<sol::state>();
        lua_state->open_libraries(sol::lib::base, sol::lib::coroutine,
                                  sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::utf8);

        lua_state->globals()["load"] = sol::nil;
        lua_state->globals()["loadfile"] = sol::nil;
        lua_state->globals()["dofile"] = sol::nil;

        bindings::lua::LuaBinder::register_bindings(*lua_state);
        return lua_state;
    }
} // namespace engine::execution::script
