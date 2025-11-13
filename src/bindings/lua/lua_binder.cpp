/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file lua_binder.cpp
 * @brief Implements the LuaBinder utility for registering Lua bindings.
 */

#include "bindings/lua/lua_binder.hpp"

#include <chrono>
#include <exception>
#include <iostream>

namespace engine::bindings::lua
{
    /**
     * @brief Registers all Lua bindings to the given Lua state. Logs the registration duration and any errors encountered.
     * @param lua The Lua state to register bindings to.
     */
    void LuaBinder::register_bindings(sol::state& lua)
    {
        try
        {
            const auto start_time = std::chrono::high_resolution_clock::now();

            register_members(lua);
            register_non_members(lua);
            register_manual_bindings(lua);

            const auto end_time = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            std::cout << "Lua bindings successfully registered in " << duration.count() << "in milliseconds" << std::endl;
        } catch (const sol::error& e)
        {
            std::cerr << "Error registering Lua bindings: " << e.what() << std::endl;
        } catch (const std::exception& e)
        {
            std::cerr << "Unhandled exception while registering Lua bindings: " << e.what() << std::endl;
        }
    }
} // namespace engine::bindings::lua
