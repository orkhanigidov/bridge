#pragma once

#include "pch.hpp"

namespace engine::lua_bindings
{
    template <typename T>
    class class_registrar final
    {
      public:
        explicit class_registrar(sol::state& lua, std::string_view name) : lua_(lua)
        {
            usertype_ = lua_.new_usertype<T>(name, sol::no_constructor);
        }

        template <typename... CtorArgs>
        class_registrar& add_constructor()
        {
            usertype_.set(sol::meta_function::construct, sol::constructors<T(CtorArgs...)>());
            return *this;
        }

        template <typename Fn>
        class_registrar& add_function(std::string_view name, Fn&& fn)
        {
            usertype_.set_function(name, std::forward<Fn>(fn));
            return *this;
        }

        template <typename Base>
        class_registrar& add_base_class()
        {
            usertype_.set(sol::base_classes, sol::bases<Base>());
            return *this;
        }

        class_registrar& enable_garbage_collector()
        {
            usertype_.set(sol::meta_function::garbage_collect, [] {});
            return *this;
        }

        // TODO
        void finalize() {}

      private:
        sol::state& lua_;
        sol::usertype<T> usertype_;
    };
} // namespace engine::lua_bindings