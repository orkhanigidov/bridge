#include "bindings/lua/lua_binder.hpp"

#include <chrono>
#include <exception>
#include <format>
#include <iostream>

namespace engine::bindings::lua
{
    void LuaBinder::register_bindings(sol::state& lua)
    {
        try
        {
            const auto start_time = std::chrono::high_resolution_clock::now();

            register_members(lua);
            register_non_members(lua);

            const auto end_time = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            std::cout << std::format("Lua bindings successfully registered in {} in milliseconds\n", duration.count());
        } catch (const sol::error& e)
        {
            std::cerr << std::format("Error registering Lua bindings: {}\n", e.what());
        }
        catch (const std::exception& e)
        {
            std::cerr << std::format("Unhandled exception while registering Lua bindings: {}\n", e.what());
        }
    }
} // namespace engine::bindings::lua
