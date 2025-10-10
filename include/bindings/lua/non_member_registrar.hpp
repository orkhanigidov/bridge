#pragma once

#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    class NonMemberRegistrar final
    {
    public:
        explicit NonMemberRegistrar(sol::state& lua) : lua_(lua)
        {
        }

        template <typename F>
            requires !std::is_member_function_pointer_v<std::remove_cvref_t<F>>
        NonMemberRegistrar& function(std::string name, F&& f)
        {
            lua_.set_function(std::move(name), std::forward<F>(f));
            return *this;
        }

    private:
        sol::state& lua_;
    };
} // namespace engine::bindings::lua
