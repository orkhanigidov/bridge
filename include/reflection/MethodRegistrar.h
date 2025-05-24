#pragma once

#include "../pch.h"

namespace engine::reflection
{
    class MethodRegistrar
    {
      public:
        template <typename Func>
        static void registerGlobalMethod(const std::string& name, Func func,
                                         const std::vector<std::string>& parameterNames,
                                         const std::string& category = "", const std::string& description = "")
        {
            rttr::registration::method(name, func)(rttr::parameter_names(joinParameterNames(parameterNames)),
                                                   rttr::metadata("category", category),
                                                   rttr::metadata("description", description));
        }

        template <typename Class, typename Func>
        static void registerMemberMethod(const std::string& name, Func func,
                                         const std::vector<std::string>& parameterNames,
                                         const std::string& category = "", const std::string& description = "")
        {
            const std::string className = rttr::type::get<Class>().get_name();

            rttr::registration::class_<Class>(className).method(name, func)(
                rttr::parameter_names(joinParameterNames(parameterNames)), rttr::metadata("category", category),
                rttr::metadata("description", description));
        }

      private:
        static std::string joinParameterNames(const std::vector<std::string>& parameterNames)
        {
            std::string result;
            for (const auto& parameterName : parameterNames)
            {
                if (!result.empty())
                    result += ", ";

                result += parameterName;
            }

            return result;
        }
    };
} // namespace engine::reflection