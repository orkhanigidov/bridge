/**
 * Project: Engine
 * File: lua_state_manager.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file lua_state_manager.hpp
 * @brief Declares the LuaStateManager utility for managing a thread-local Lua state.
 */

#pragma once

#include <memory>
#include <sol/state.hpp>

namespace engine::execution::script
{
    /**
     * @class LuaStateManager
     * @brief Utility class for managing a thread-local Lua state.
     */
    class LuaStateManager final
    {
    public:
        /**
         * @brief Returns a reference to the thread-local Lua state. Initializes the state if it does not exist for the current thread.
         * @return Reference to the thread-local Lua state.
         */
        static sol::state& get_state()
        {
            thread_local sol::state* state_ = nullptr;
            if (state_ == nullptr)
            {
                initialize_thread_state(state_);
            }
            return *state_;
        }

    private:
        /**
         * @brief Initializes the thread-local Lua state.
         * @param state Reference to the unique pointer holding the Lua state.
         */
        static void initialize_thread_state(sol::state*& state);
    };
} // namespace engine::execution::script
