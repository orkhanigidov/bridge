/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <memory>
#include <sol/state.hpp>

namespace engine::execution::script
{
    class LuaStateManager final
    {
    public:
        static sol::state& get_state()
        {
            thread_local std::unique_ptr<sol::state> state_;
            if (!state_)
            {
                initialize_thread_state(state_);
            }
            return *state_;
        }

    private:
        static void initialize_thread_state(std::unique_ptr<sol::state>& state);
    };
} // namespace engine::execution::script
