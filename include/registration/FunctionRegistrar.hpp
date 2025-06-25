#pragma once

#include "FunctionTraits.hpp"
#include "model/ClassDesc.hpp"
#include "model/FuncDesc.hpp"
#include "pch.hpp"
#include "sol/sol.hpp"

namespace engine::reflection
{
    class FunctionRegistrar final
    {
      public:
        template <typename Func>
        static std::unique_ptr<model::FuncDesc>
        register_global_func(sol::state& lua, const std::string& name, Func func,
                             const std::string& category = "", const std::string& description = "")
        {
            lua[name] = func;

            auto return_type = extract_return_type<Func>();
            auto param_types = extract_param_types<Func>();
            auto param_names = extract_param_names<Func>();

            std::vector<model::ParamDesc> parameters;
            for (size_t i = 0; i < param_names.size(); ++i)
            {
                parameters.emplace_back(param_names[i], param_types[i], false, "");
            }

            return std::make_unique<model::FuncDesc>(name, return_type, parameters, category,
                                                     description);
        }

        template <typename T, typename Func>
        static void register_member_func(sol::usertype<T>& usertype, const std::string& name,
                                         model::ClassDesc& class_info, Func func,
                                         const std::string& category    = "",
                                         const std::string& description = "")
        {
            usertype[name] = func;

            auto return_type = extract_return_type<Func>();
            auto param_types = extract_param_types<Func>();
            auto param_names = extract_param_names<Func>();

            std::vector<model::ParamDesc> parameters;
            for (size_t i = 0; i < param_names.size(); ++i)
            {
                parameters.emplace_back(param_names[i], param_types[i], false, "");
            }

            class_info.add_function(
                model::FuncDesc(name, return_type, parameters, category, description));
        }
    };
} // namespace engine::reflection