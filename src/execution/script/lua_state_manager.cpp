/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file lua_state_manager.cpp
 * @brief Implements the LuaStateManager utility for managing a thread-local Lua state.
 */

#include "execution/script/lua_state_manager.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <sol/state.hpp>

#include "bindings/lua/lua_binder.hpp"

namespace engine::execution::script
{
    /**
     * @brief Initializes the thread-local Lua state. Opens standard libraries and registers custom bindings. Logs initialization success or failure.
     * @param state Reference to the uniques pointer holding the Lua state.
     */
    void LuaStateManager::initialize_thread_state(std::unique_ptr<sol::state>& state)
    {
        try
        {
            state = std::make_unique<sol::state>();
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
