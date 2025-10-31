/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    class LuaBinder final
    {
    public:
        LuaBinder() = delete;

        static void register_bindings(sol::state& lua);
    };

    void register_members(sol::state& lua);
    void register_non_members(sol::state& lua);
} // namespace engine::bindings::lua
