#pragma once

#include <sol/sol.hpp>

namespace bindings::lua {

    template<typename T>
    class MemberRegistrar final {
    public:
        explicit MemberRegistrar(sol::state& lua, std::string name):
            m_lua(lua),
            m_usertype(m_lua.new_usertype<T>(std::move(name), sol::no_constructor)) {}

        template<typename... Args>
        MemberRegistrar& constructor()
        {
            m_usertype.set(sol::meta_function::construct, sol::constructors<T(Args...)>());
            return *this;
        }

        template<typename... Bases>
        MemberRegistrar& bases()
        {
            m_usertype.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

        template<typename V>
        MemberRegistrar& variable(std::string name, V&& v)
        {
            m_usertype.set(std::move(name), std::forward<V>(v));
            return *this;
        }

        template<std::invocable F>
            requires std::is_member_function_pointer_v<std::remove_cvref_t<F>>
        MemberRegistrar& function(std::string name, F&& f)
        {
            m_usertype.set_function(std::move(name), std::forward<F>(f));
            return *this;
        }

        MemberRegistrar& enable_garbage_collection()
        {
            m_usertype.set(sol::meta_function::garbage_collect, /* [](T &obj) { obj.~T(); }, */ sol::default_destructor);
            return *this;
        }

    private:
        sol::state& m_lua;
        sol::usertype<T> m_usertype;
    };

} // namespace bindings::lua
