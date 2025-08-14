#pragma once

#include <sol/sol.hpp>

namespace bindings::lua {
    template<typename T> class ClassRegistrar final {
    public:
        explicit ClassRegistrar(sol::state& lua, std::string class_name)
            : m_lua(lua), m_usertype(m_lua.new_usertype<T>(
                                  std::move(class_name), sol::no_constructor))
        {}

        template<typename... Args> ClassRegistrar& constructor()
        {
            m_usertype.set(sol::meta_function::construct,
                           sol::constructors<T(Args...)>());
            return *this;
        }

        template<typename... Bases> ClassRegistrar& base_classes()
        {
            m_usertype.set(sol::base_classes, sol::bases<Bases...>());
            return *this;
        }

        template<typename Var>
        ClassRegistrar& variable(std::string name, Var&& var)
        {
            m_usertype.set(std::move(name), std::forward<Var>(var));
            return *this;
        }

        template<typename Func>
        ClassRegistrar& method(std::string name, Func&& func)
        {
            m_usertype.set_function(std::move(name), std::forward<Func>(func));
            return *this;
        }

        template<typename Key, typename Value>
        ClassRegistrar& set(Key&& key, Value&& value)
        {
            m_usertype.set(std::forward<Key>(key), std::forward<Value>(value));
            return *this;
        }

        ClassRegistrar& enable_gc()
        {
            m_usertype.set(sol::meta_function::garbage_collect,
                           sol::default_destructor);
            return *this;
        }

    private:
        sol::state& m_lua;
        sol::usertype<T> m_usertype;
    };
} // namespace bindings::lua
