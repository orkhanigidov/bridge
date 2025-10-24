#pragma once

#include <string>
#include <type_traits>
#include <utility>
#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    class NonMemberRegistrar final
    {
    public:
        explicit NonMemberRegistrar(sol::state& lua) : lua_(lua)
        {
        }

        sol::table create_namespace(const std::string& name) const
        {
            return lua_.create_named_table(name);
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

        template <typename Var>
        NonMemberRegistrar& add_variable(const std::string& name, Var&& var, bool is_readonly = false)
        {
            if (is_readonly)
            {
                lua_.set(name, sol::readonly(std::forward<Var>(var)));
            }
            else
            {
                lua_.set(name, std::forward<Var>(var));
            }
            return *this;
        }

        template <typename... Args>
        NonMemberRegistrar& add_enums(const std::string& name, Args&&... args)
        {
            lua_.new_enum(name, std::forward<Args>(args)...);
            return *this;
        }

    private:
        sol::state& lua_;
    };
} // namespace engine::bindings::lua
