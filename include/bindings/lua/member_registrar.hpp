/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file member_registrar.hpp
 * @brief Declares the MemberRegistrar utility for registering C++ members to Lua.
 */

#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <sol/sol.hpp>

#include "memory_ownership.hpp"

namespace engine::bindings::lua
{
    /**
     * @class MemberRegistrar
     * @brief Utility class for registering C++ class members, functions, and properties to Lua.
     * @tparam T The C++ class type to register.
     * @tparam Ownership Specifies whether memory is managed by C++ or Lua.
     */
    template <typename T, MemoryOwnership Ownership>
    class MemberRegistrar final
    {
    public:
        /**
         * @brief Constructs a MemberRegistrar and registers the usertype in Lua.
         * @param lua The Lua state.
         * @param name The name of the usertype in Lua.
         */
        explicit MemberRegistrar(sol::state& lua, const std::string& name) : lua_(lua)
        {
            usertype_ = lua.new_usertype<T>(name);
            table_ = lua[name].get<sol::table>();

            if constexpr (Ownership == MemoryOwnership::Cpp)
            {
                usertype_[sol::meta_function::garbage_collect] = [](T&)
                {
                };
            }
        }

        /**
         * @brief Adds constructors for the usertype (Lua ownership).
         * @tparam TSignatures Constructor signatures.
         * @return Reference to this registrar.
         */
        template <typename... TSignatures>
        MemberRegistrar& add_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::constructors<TSignatures...>());
            }
            return *this;
        }

        /**
         * @brief Adds raw pointer constructors (C++ ownership).
         * @tparam TSignatures Constructor signatures.
         * @return Reference to this registrar.
         */
        template <typename... TSignatures>
        MemberRegistrar& add_raw_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Cpp)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_raw_call_factory(TSignatures{})...));
            }
            return *this;
        }

        /**
         * @brief Adds unique pointer constructors (Lua ownership).
         * @tparam TSignatures Constructor signatures.
         * @return Reference to this registrar.
         */
        template <typename... TSignatures>
        MemberRegistrar& add_unique_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_unique_call_factory(TSignatures{})...));
            }
            return *this;
        }

        /**
         * @brief Adds shared pointer constructors (Lua ownership).
         * @tparam TSignatures Constructor signatures.
         * @return References to this registrar.
         */
        template <typename... TSignatures>
        MemberRegistrar& add_shared_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_shared_call_factory(TSignatures{})...));
            }
            return *this;
        }

        /**
         * @brief Adds a custom destructor (Lua ownership).
         * @tparam F Destructor function type.
         * @param f The destructor function.
         * @return Reference to this registrar.
         */
        template <typename F>
        MemberRegistrar& add_destructor(F&& f)
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_[sol::meta_function::garbage_collect] = sol::destructor(std::forward<F>(f));
            }
            return *this;
        }

        /**
         * @brief Adds base classes to the usertype.
         * @tparam Bases Base class types.
         * @return Reference to this registrar.
         */
        template <typename... Bases>
        MemberRegistrar& add_bases()
        {
            usertype_.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

        /**
         * @brief Adds a member variable to the usertype.
         * @tparam Var Variable type.
         * @param name The variable name in Lua.
         * @param var Pointer to member variable.
         * @param is_readonly Whether the variable is read-only.
         * @return Reference to this registrar.
         */
        template <typename Var>
        MemberRegistrar& add_variable(const std::string& name, Var T::* var, bool is_readonly = false)
        {
            if (is_readonly)
            {
                usertype_.set(name, sol::readonly(var));
            }
            else
            {
                usertype_.set(name, var);
            }
            return *this;
        }

        /**
         * @brief Adds a static variable to the usertype.
         * @tparam Var Variable type.
         * @param name The variable name in Lua.
         * @param var The static variable.
         * @return Reference to this registrar.
         */
        template <typename Var>
        MemberRegistrar& add_static_variable(const std::string& name, Var&& var)
        {
            usertype_.set(name, sol::var(std::forward<Var>(var)));
            return *this;
        }

        /**
         * @brief Adds an enum to the Lua state.
         * @tparam E Enum type.
         * @param name The enum name in Lua.
         * @param items List of enum name-value pairs.
         * @return Reference to this registrar.
         */
        template <typename E>
        MemberRegistrar& add_enums(const std::string& name, std::initializer_list<std::pair<std::string_view, E>> items)
        {
            (void)name;

            for (const auto& pair : items)
            {
                lua_.set(pair.first, pair.second);
            }
            // table_.new_enum<E>(name, items);
            return *this;
        }

        /**
         * @brief Adds a property with getter and setter.
         * @tparam Getter Getter function type.
         * @tparam Setter Setter function type.
         * @param name The property name in Lua.
         * @param getter The getter function.
         * @param setter The setter function.
         * @return Reference to this registrar.
         */
        template <typename Getter, typename Setter>
        MemberRegistrar& add_property(const std::string& name, Getter&& getter, Setter&& setter)
        {
            usertype_.set(name, sol::property(std::forward<Getter>(getter), std::forward<Setter>(setter)));
            return *this;
        }

        /**
         * @brief Adds a read-only property.
         * @tparam Getter Getter function type.
         * @param name The property name in Lua.
         * @param getter The getter function.
         * @return Reference to this registrar.
         */
        template <typename Getter>
        MemberRegistrar& add_readonly_property(const std::string& name, Getter&& getter)
        {
            usertype_.set(name, sol::readonly_property(std::forward<Getter>(getter)));
            return *this;
        }

        /**
         * @brief Adds a member function or overloads.
         * @tparam Fs Member function pointer types.
         * @param name The function name in Lua.
         * @param fs The member function pointers.
         * @return Reference to this registrar.
         */
        template <typename... Fs>
            requires (std::is_member_function_pointer_v<std::remove_cvref_t<Fs>> && ...)
        MemberRegistrar& add_function(const std::string& name, Fs&&... fs)
        {
            if constexpr (sizeof...(Fs) == 1)
            {
                usertype_.set_function(name, std::forward<Fs>(fs)...);
            }
            else
            {
                usertype_.set_function(name, sol::overload(std::forward<Fs>(fs)...));
            }
            return *this;
        }

        /**
         * @brief Adds a static member function or overloads.
         * @tparam Fs Static function pointer types.
         * @param name The function name in Lua.
         * @param fs The static function pointers.
         * @return Reference to this registrar.
         */
        template <typename... Fs>
            requires (std::is_member_function_pointer_v<std::remove_cvref_t<Fs>> && ...)
        MemberRegistrar& add_static_function(const std::string& name, Fs&&... fs)
        {
            if constexpr (sizeof...(Fs) == 1)
            {
                usertype_.set_static_function(name, std::forward<Fs>(fs)...);
            }
            else
            {
                usertype_.set_static_function(name, sol::overload(std::forward<Fs>(fs)...));
            }
            return *this;
        }

        template <typename... Fs>
        MemberRegistrar& add_wrapper_function(const std::string& name, Fs&&... fs)
        {
            if constexpr (sizeof...(Fs) == 1)
            {
                usertype_.set_function(name, std::forward<Fs>(fs)...);
            }
            else
            {
                usertype_.set_function(name, sol::overload(std::forward<Fs>(fs)...));
            }
            return *this;
        }

        /**
         * @brief Adds a meta function to the usertype.
         * @tparam F Function type.
         * @param key The meta function key.
         * @param f The function.
         * @return Reference to this registrar.
         */
        template <typename F>
        MemberRegistrar& add_meta_function(sol::meta_function key, F&& f)
        {
            usertype_[key] = std::forward<F>(f);
            return *this;
        }

    private:
        /**
         * @brief Reference to the Lua state.
         */
        sol::state& lua_;

        /**
         * @brief The usertype registered in Lua.
         */
        sol::usertype<T> usertype_;

        /**
         * @brief The table for static members and enums
         */
        sol::table table_;

        template <typename... Args>
            requires (std::is_constructible_v<T, Args...>)
        static auto create_raw_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return new T(std::forward<Args>(args)...);
            };
        }

        template <typename... Args>
            requires (std::is_constructible_v<T, Args...>)
        static auto create_unique_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return std::make_unique<T>(std::forward<Args>(args)...);
            };
        }

        template <typename... Args>
            requires (std::is_constructible_v<T, Args...>)
        static auto create_shared_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return std::make_shared<T>(std::forward<Args>(args)...);
            };
        }
    };
} // namespace engine::bindings::lua
