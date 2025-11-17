/**
 * Project: Engine
 * File: lua_binder.hpp
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
 * @file lua_binder.hpp
 * @brief Declares the LuaBinder utility for registering Lua bindings.
 */

#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    /**
     * @class LuaBinder
     * @brief Utility class for registering all Lua bindings.
     */
    class LuaBinder final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        LuaBinder() = delete;

        /**
         * @brief Registers all Lua bindings to the given Lua state.
         * @param lua The Lua state to register bindings to.
         */
        static void register_bindings(sol::state& lua);
    };

    /**
     * @brief Registers member functions and classes to the Lua state.
     * @param lua The Lua state to register member bindings to.
     */
    void register_members(sol::state& lua);

    /**
     * @brief Registers non-member functions to the Lua state.
     * @param lua The Lua state to register non-member bindings to.
     */
    void register_non_members(sol::state& lua);

    /**
     * @brief Registers manually defined Lua bindings.
     * @param lua The Lua state to register bindings to.
     */
    void register_manual_bindings(sol::state& lua);
} // namespace engine::bindings::lua
