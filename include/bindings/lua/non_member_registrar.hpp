/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file non_member_registrar.hpp
 * @brief Declares the NonMemberRegistrar utility for registering non-member functions and variables to Lua.
 */

#pragma once

#include <initializer_list>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    /**
     * @class NonMemberRegistrar
     * @brief Utility class for registering non-member functions, variables, and enums to Lua.
     */
    class NonMemberRegistrar final
    {
    public:
        /**
         * @brief Constructs a NonMemberRegistrar for the given Lua state.
         * @param lua The Lua state.
         */
        explicit NonMemberRegistrar(sol::state& lua) : lua_(lua)
        {
        }

        /**
         * @brief Creates a name namespace (table) in Lua.
         * @param name The name of the namespace.
         * @return The created Lua table.
         */
        sol::table create_namespace(const std::string& name) const
        {
            return lua_.create_named_table(name);
        }

        /**
         * @brief Adds a non-member function or overloads to Lua.
         * @tparam Fs Function types (must not be member function pointers).
         * @param name The function name in Lua.
         * @param fs The function(s) to register.
         * @return Reference to this registrar.
         */
        template <typename... Fs>
        NonMemberRegistrar& add_function(const std::string& name, Fs&&... fs)
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

        /**
         * @brief Adds a variable to Lua, optionally as read-only.
         * @tparam Var Variable type.
         * @param name The variable name in Lua.
         * @param var The variable to register.
         * @param is_readonly Whether the variable is read-only.
         * @return Reference to this registrar.
         */
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

        /**
         * @brief Adds an enum to Lua.
         * @tparam E Enum type.
         * @param name The enum name is Lua.
         * @param items List of enum name-value pairs.
         * @return Reference to this registrar.
         */
        template <typename E>
        NonMemberRegistrar& add_enums(const std::string& name, std::initializer_list<std::pair<std::string_view, E>> items)
        {
            (void)name;

            for (const auto& pair : items)
            {
                lua_.set(pair.first, pair.second);
            }
            // lua_.new_enum<E>(name, items);
            return *this;
        }

    private:
        /**
         * @brief Reference to the Lua state.
         */
        sol::state& lua_;
    };
} // namespace engine::bindings::lua
