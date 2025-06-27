#pragma once

#include "pch.hpp"

namespace engine::registration
{
    template <typename T>
    class ClassRegistrar
    {
      public:
        ClassRegistrar(sol::state& lua, std::string_view name) : lua_(lua)
        {
            usertype_ = lua_.new_usertype<T>(name, sol::no_constructor);
        }

        template <typename... CtorArgs>
        ClassRegistrar& ctor()
        {
            usertype_.set(sol::meta_function::construct, sol::constructors<T(CtorArgs...)>());
            return *this;
        }

        template <typename Fn>
        ClassRegistrar& func(std::string_view name, Fn&& fn)
        {
            usertype_.set_function(name, std::forward<Fn>(fn));
            return *this;
        }

        template <typename Base>
        ClassRegistrar& base()
        {
            usertype_.set(sol::base_classes, sol::bases<Base>());
            return *this;
        }

        ClassRegistrar& gc()
        {
            usertype_.set(sol::meta_function::garbage_collect, [] {});
            return *this;
        }

        void register_class() {}

      private:
        sol::state& lua_;
        sol::usertype<T> usertype_;
    };
} // namespace engine::registration