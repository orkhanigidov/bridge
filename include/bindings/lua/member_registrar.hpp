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
    template <typename T, MemoryOwnership Ownership>
    class MemberRegistrar final
    {
    public:
        explicit MemberRegistrar(sol::state& lua, const std::string& name) : lua_(lua)
        {
            usertype_ = lua.new_usertype<T>(name);

            if constexpr (Ownership == MemoryOwnership::Cpp)
            {
                usertype_[sol::meta_function::garbage_collect] = [](T&)
                {
                };
            }
        }

        template <typename... TSignatures>
        MemberRegistrar& add_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::constructors<TSignatures...>());
            }
            return *this;
        }

        template <typename... TSignatures>
        MemberRegistrar& add_raw_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Cpp)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_raw_call_factory(TSignatures{})...));
            }
            return *this;
        }

        template <typename... TSignatures>
        MemberRegistrar& add_unique_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_unique_call_factory(TSignatures{})...));
            }
            return *this;
        }

        template <typename... TSignatures>
        MemberRegistrar& add_shared_constructors()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_shared_call_factory(TSignatures{})...));
            }
            return *this;
        }

        template <typename F>
        MemberRegistrar& add_destructor(F&& f)
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_[sol::meta_function::garbage_collect] = sol::destructor(std::forward<F>(f));
            }
            return *this;
        }

        template <typename... Bases>
        MemberRegistrar& add_bases()
        {
            usertype_.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

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

        template <typename Var>
        MemberRegistrar& add_static_variable(const std::string& name, Var&& var)
        {
            usertype_.set(name, sol::var(std::forward<Var>(var)));
            return *this;
        }

        template <typename E>
        MemberRegistrar& add_enums(const std::string& name, std::initializer_list<std::pair<std::string_view, E>> items)
        {
            lua_.new_enum<E>(name, items);
            return *this;
        }

        template <typename Getter, typename Setter>
        MemberRegistrar& add_property(const std::string& name, Getter&& getter, Setter&& setter)
        {
            usertype_.set(name, sol::property(std::forward<Getter>(getter), std::forward<Setter>(setter)));
            return *this;
        }

        template <typename Getter>
        MemberRegistrar& add_readonly_property(const std::string& name, Getter&& getter)
        {
            usertype_.set(name, sol::readonly_property(std::forward<Getter>(getter)));
            return *this;
        }

        template <typename... Fs>
            requires (std::is_member_function_pointer_v<std::remove_cvref_t<Fs>> && ...)
        MemberRegistrar& add_functions(const std::string& name, Fs&&... fs)
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

        template <typename... Fs>
            requires (std::is_member_function_pointer_v<std::remove_cvref_t<Fs>> && ...)
        MemberRegistrar& add_static_functions(const std::string& name, Fs&&... fs)
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

        template <typename F>
        MemberRegistrar& add_meta_function(sol::meta_function key, F&& f)
        {
            usertype_[key] = std::forward<F>(f);
            return *this;
        }

    private:
        sol::state& lua_;
        sol::usertype<T> usertype_;

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        static auto create_raw_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return new T(std::forward<Args>(args)...);
            };
        }

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        static auto create_unique_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return std::make_unique<T>(std::forward<Args>(args)...);
            };
        }

        template <typename... Args>
            requires std::is_constructible_v<T, Args...>
        static auto create_shared_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return std::make_shared<T>(std::forward<Args>(args)...);
            };
        }
    };
} // namespace engine::bindings::lua
