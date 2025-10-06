#pragma once

#include "memory_ownership.hpp"
#include <sol/sol.hpp>

namespace engine::bindings::lua
{
    template <typename T>
    class MemberRegistrar final
    {
    public:
        explicit MemberRegistrar(sol::state& lua, std::string name)
            : m_lua(lua), m_usertype(m_lua.new_usertype<T>(std::move(name), sol::no_constructor))
        {
        }

        template <typename... Args>
        MemberRegistrar& constructor()
        {
            m_usertype.set(sol::call_constructor, sol::constructors<T(Args...)>());
            return *this;
        }

        template <typename... Args>
        MemberRegistrar& shared_constructor()
        {
            m_usertype.set(sol::call_constructor, sol::factories([](Args... args)
            {
                return std::make_shared<T>(args...);
            }));
            return *this;
        }

        template <typename... Fs>
        MemberRegistrar& factories(Fs&&... fs)
        {
            m_usertype.set(sol::meta_function::construct, sol::factories(std::forward<Fs>(fs)...));
            return *this;
        }

        template <typename... Bases>
        MemberRegistrar& bases()
        {
            m_usertype.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

        template <typename V>
        MemberRegistrar& variable(std::string name, V&& v)
        {
            m_usertype.set(std::move(name), sol::var(std::forward<V>(v)));
            return *this;
        }

        template <typename F>
            requires std::is_member_function_pointer_v<std::remove_cvref_t<F>>
        MemberRegistrar& function(std::string name, F&& f)
        {
            m_usertype.set_function(std::move(name), std::forward<F>(f));
            return *this;
        }

        MemberRegistrar& set_ownership(MemoryOwnership ownership)
        {
            if (ownership == MemoryOwnership::Cpp)
            {
                m_usertype.set(sol::meta_function::garbage_collect, sol::destructor([](T&)
                {
                }));
            }
            return *this;
        }

    private:
        sol::state& m_lua;
        sol::usertype<T> m_usertype;
    };
} // namespace engine::bindings::lua
