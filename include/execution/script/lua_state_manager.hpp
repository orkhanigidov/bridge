#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <sol/state.hpp>

namespace engine::execution::script
{
    class LuaStateManager final
    {
    public:
        static sol::state& get_state()
        {
            std::call_once(init_flag_, &LuaStateManager::initialize);
            if (!state_)
            {
                throw std::runtime_error("Global Lua state is not initialized.");
            }
            return *state_;
        }

    private:
        static void initialize();
        static std::unique_ptr<sol::state> state_;
        static std::once_flag init_flag_;
    };
} // namespace engine::execution::script
