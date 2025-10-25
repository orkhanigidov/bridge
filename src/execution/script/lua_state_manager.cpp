#include "execution/script/lua_state_manager.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <sol/state.hpp>

#include "bindings/lua/lua_binder.hpp"

namespace engine::execution::script
{
    std::unique_ptr<sol::state> LuaStateManager::state_ = nullptr;
    std::once_flag LuaStateManager::init_flag_;

    void LuaStateManager::initialize()
    {
        try
        {
            state_ = std::make_unique<sol::state>();
            state_->open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os,
                                   sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::utf8);

            bindings::lua::LuaBinder::register_bindings(*state_);

            std::cout << "Global Lua state initialized." << std::endl;
        } catch (const std::exception& e)
        {
            std::cerr << "Failed to initialize global Lua state: " << e.what() << std::endl;
            state_ = nullptr;
        }
    }
} // namespace engine::execution::script
