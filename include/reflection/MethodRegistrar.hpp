#pragma once

#include "pch.hpp"

namespace engine::reflection
{
    class MethodRegistrar
    {
      public:
        template <typename Func, typename... ParamNames>
        static void
        register_global_method(rttr::string_view name, Func func, rttr::string_view category = "",
                               rttr::string_view description = "", ParamNames&&... parameterNames)
        {
            // TODO: Check if arg_count matches parameter names count
            static_assert(sizeof...(ParamNames) == rttr::detail::function_traits<Func>::arg_count,
                          "Parameter name count must match function argument count");

            rttr::registration::method(name, func)(
                rttr::parameter_names(std::forward<ParamNames>(parameterNames)...),
                rttr::metadata("category", category), rttr::metadata("description", description));
        }

        template <typename Class, typename Func, typename... ParamNames>
        static void register_member_method(rttr::string_view class_name, rttr::string_view name,
                                           Func func, rttr::string_view category = "",
                                           rttr::string_view description = "",
                                           ParamNames&&... parameterNames)
        {
            // TODO: Check if arg_count matches parameter names count
            static_assert(sizeof...(ParamNames) == rttr::detail::function_traits<Func>::arg_count,
                          "Parameter name count must match function argument count");

            rttr::registration::class_<Class>(class_name)
                .method(name,
                        func)(rttr::parameter_names(std::forward<ParamNames>(parameterNames)...),
                              rttr::metadata("category", category),
                              rttr::metadata("description", description));
        }
    };
} // namespace engine::reflection