#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    class LuaBinder final
    {
    public:
        LuaBinder() = default;

        void register_bindings(sol::state& lua);
    };

    void register_members(sol::state& lua);
    void register_non_members(sol::state& lua);
} // namespace engine::bindings::lua
