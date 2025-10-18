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

        template <typename... Fs>
            requires (!std::is_member_function_pointer_v<std::remove_cvref_t<Fs>> && ...)
        NonMemberRegistrar& add_functions(const std::string& name, Fs&&... fs)
        {
            if constexpr (sizeof...(Fs) == 1)
            {
                lua_.set_function(name, std::forward<Fs>(fs)...);
            }
            else
            {
                lua_.set_function(name, sol::overload(std::forward<Fs>(fs)...));
            }
            return *this;
        }

        template <typename E, typename... Args>
        NonMemberRegistrar& add_enums(const std::string& name, Args&&... args)
        {
            lua_.new_enum<E>(name, std::forward<Args>(args)...);
            return *this;
        }

    private:
        sol::state& lua_;
    };
} // namespace engine::bindings::lua
