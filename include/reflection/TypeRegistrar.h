#pragma once

#include "../pch.h"

namespace engine::reflection
{
    class TypeRegistrar
    {
      public:
        template <typename Class, typename... Args>
        static void register_type(std::string_view id, std::string_view name)
        {
            auto registration = rttr::registration::class_<Class>(name)(rttr::metadata("id", id));

            if constexpr (sizeof...(Args) == 0)
                registration.constructor()(rttr::policy::ctor::as_object);
            else
                registration.template constructor<Args...>()(rttr::policy::ctor::as_object);
        }
    };
} // namespace engine::reflection