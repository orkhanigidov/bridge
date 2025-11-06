/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
            thread_local std::unique_ptr<sol::state> state_;
            if (!state_)
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
        static void initialize_thread_state(std::unique_ptr<sol::state>& state);
    };
} // namespace engine::execution::script
