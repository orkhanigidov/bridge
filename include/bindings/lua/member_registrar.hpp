#pragma once

#include "memory_ownership.hpp"

#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    template <typename T, MemoryOwnership Ownership>
    class MemberRegistrar final
    {
    public:
        explicit MemberRegistrar(sol::state& lua, const std::string& name)
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_ = lua.new_usertype<T>(name);
            }
            else
            {
                usertype_ = lua.new_usertype<T>(name, sol::no_constructor);
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
        MemberRegistrar& set_call_constructor()
        {
            if constexpr (Ownership == MemoryOwnership::Lua)
            {
                usertype_.set(sol::call_constructor, sol::factories(create_call_factory(TSignatures{})...));
            }
            return *this;
        }

        template <typename... Bases>
        MemberRegistrar& add_bases()
        {
            usertype_.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

        template <typename V>
        MemberRegistrar& add_variable(const std::string& name, V T::* v, bool is_readonly = false)
        {
            if (is_readonly)
            {
                usertype_.set(name, sol::readonly(v));
            }
            else
            {
                usertype_.set(name, v);
            }
            return *this;
        }

        template <typename V>
        MemberRegistrar& add_static_variable(const std::string& name, V&& v)
        {
            usertype_.set(name, sol::var(std::forward<V>(v)));
            return *this;
        }

        template <typename... Fs>
            requires std::conjunction_v<std::is_member_function_pointer<std::remove_cvref_t<Fs>>...>
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

    private:
        sol::usertype<T> usertype_;

        template <typename... Args>
        static auto create_call_factory(sol::types<Args...>)
        {
            return [](Args... args)
            {
                return std::make_shared<T>(args...);
            };
        }
    };
} // namespace engine::bindings::lua
