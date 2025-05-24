#pragma once

#include "../pch.h"

namespace engine::reflection
{
    class TypeRegistrar
    {
      public:
        template <typename Class>
        static void registerType(const std::string& id, const std::string& name)
        {
            rttr::registration::class_<Class>(name).constructor()(rttr::policy::ctor::as_object)(
                rttr::metadata("id", id));
        }

        template <typename Class, typename... Args>
        static void registerType(const std::string& id, const std::string& name)
        {
            rttr::registration::class_<Class>(name).constructor<Args...>()(rttr::policy::ctor::as_object)(
                rttr::metadata("id", id));
        }
    };
} // namespace engine::reflection