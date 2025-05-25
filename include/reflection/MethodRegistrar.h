#pragma once

#include "../pch.h"

namespace engine::reflection
{
    class MethodRegistrar
    {
      public:
        template <typename Func, typename... ParamNames>
        static void registerGlobalMethod(const std::string& name, Func func, const std::string& category = "",
                                         const std::string& description = "", ParamNames&&... parameterNames)
        {
            static_assert(sizeof...(ParamNames) == rttr::detail::function_traits<Func>::arg_count,
                          "Parameter name count must match function argument count");

            rttr::registration::method(name, func)(rttr::parameter_names(parameterNames...),
                                                   rttr::metadata("category", category),
                                                   rttr::metadata("description", description));
        }

        template <typename Class, typename Func, typename... ParamNames>
        static void registerMemberMethod(const std::string& name, Func func, const std::string& category = "",
                                         const std::string& description = "", ParamNames&&... parameterNames)
        {
            static_assert(sizeof...(ParamNames) == rttr::detail::function_traits<Func>::arg_count,
                          "Parameter name count must match function argument count");

            const auto className = std::string{rttr::type::get<Class>().get_name()};

            rttr::registration::class_<Class>(className).method(name, func)(rttr::parameter_names(parameterNames...),
                                                                            rttr::metadata("category", category),
                                                                            rttr::metadata("description", description));
        }
    };
} // namespace engine::reflection