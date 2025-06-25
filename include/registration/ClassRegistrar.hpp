#pragma once

#include "model/ClassInfo.hpp"
#include "pch.hpp"

namespace engine::reflection
{
    class ClassRegistrar
    {
      public:
        template <typename T, typename... CtorArgs>
        static std::unique_ptr<model::ClassInfo>
        register_class(sol::state& lua, const std::string& name, const std::string& alias)
        {
            auto usertype = create_usertype<T, CtorArgs...>(lua, name);

            auto class_info = std::make_unique<model::ClassInfo>(name, alias);

            return class_info;
        }

      private:
        template <typename T, typename... CtorArgs>
        static sol::usertype<sol::table> create_usertype(sol::state& lua, const std::string& name)
        {
            if constexpr (sizeof...(CtorArgs) > 0)
            {
                return lua.new_usertype<T>(name, sol::constructors<T>());
            }
            else
            {
                return lua.new_usertype<T>(name, sol::constructors<T(CtorArgs...)>());
            }
        }

        // template<typename... CtorArgs>
        // static void add_constructor_info(model::ClassInfo& class_info)
        // {
        //     if constexpr (sizeof...(CtorArgs) > 0)
        //     {
        //     }
        // }
    };
} // namespace engine::reflection