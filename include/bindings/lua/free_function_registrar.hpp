#pragma once

#include <sol/sol.hpp>

namespace bindings::lua {
    class FreeFunctionRegistrar final {
    public:
        explicit FreeFunctionRegistrar(sol::state& lua): m_lua(lua) {}

        template<typename Func>
        requires(!std::is_member_function_pointer_v<std::decay_t<Func>>)
        FreeFunctionRegistrar& function(std::string name, Func&& func)
        {
            m_lua.set_function(std::move(name), std::forward<Func>(func));
            return *this;
        }

    private:
        sol::state& m_lua;
    };
} // namespace bindings::lua
