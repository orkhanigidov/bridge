/**
 * Project: Engine
 * File: lua_state_manager.cpp
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

#include "execution/script/lua_state_manager.hpp"

#include <exception>
#include <iostream>
#include <sol/state.hpp>

#include "bindings/lua/lua_binder.hpp"

namespace engine::execution::script
{
    /**
     * @brief Initializes the thread-local Lua state. Opens standard libraries and registers custom bindings. Logs initialization success or failure.
     * @param state Reference to the uniques pointer holding the Lua state.
     */
    void LuaStateManager::initialize_thread_state(sol::state*& state)
    {
        try
        {
            state = new sol::state();
            state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine,
                                  sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::utf8);

            bindings::lua::LuaBinder::register_bindings(*state);

            std::cout << "Thread-local Lua state initialized." << std::endl;
        } catch (const std::exception& e)
        {
            std::cerr << "Failed to initialize thread-local Lua state: " << e.what() << std::endl;
            state = nullptr;
        }
    }
} // namespace engine::execution::script
