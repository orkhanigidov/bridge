#include "bindings/lua/lua_binder.hpp"

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

            std::cout << "Lua bindings successfully registered in " << duration.count() << " milliseconds" << std::endl;
        }
        catch (const sol::error& e)
        {
            std::cerr << "Error registering Lua bindings: " << e.what() << std::endl;
        } catch (const std::exception& e)
        {
            std::cerr << "Unhandled exception while registering Lua bindings: " << e.what() << std::endl;
        }
    }
} // namespace engine::bindings::lua
