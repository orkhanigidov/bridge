#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua {

    class NonMemberRegistrar final {
    public:
        explicit NonMemberRegistrar(sol::state& lua):
            m_lua(lua) {}

        template<std::invocable F>
            requires !std::is_member_function_pointer_v<std::remove_cvref_t<F>>
        NonMemberRegistrar& function(std::string name, F&& f)
        {
            m_lua.set_function(std::move(name), std::forward<F>(f));
            return *this;
        }

    private:
        sol::state& m_lua;
    };

} // namespace engine::bindings::lua
