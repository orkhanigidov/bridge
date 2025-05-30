#pragma once

#include "pch.hpp"

namespace engine::reflection
{
    class TypeRegistrar
    {
      public:
        template <typename Class, typename... Args>
        static void register_type(rttr::string_view id, rttr::string_view name)
        {
            if constexpr (sizeof...(Args) == 0)
                rttr::registration::class_<Class>(name).constructor()(
                    rttr::policy::ctor::as_object)(rttr::metadata("id", id));
            else
                rttr::registration::class_<Class>(name).template constructor<Args...>()(
                    rttr::policy::ctor::as_object)(rttr::metadata("id", id));
        }
    };
} // namespace engine::reflection