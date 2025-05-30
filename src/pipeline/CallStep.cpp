#include "pipeline/CallStep.hpp"

#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"
#include "pipeline/Step.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"

#include <rttr/method.h>

namespace engine::pipeline
{
    CallStep::CallStep(const nlohmann::json& json) : Step(json)
    {
        parse_call_name();
    }

    void CallStep::execute()
    {
        const auto& registry = reflection::ReflectionRegistry::instance();
        const auto& pool     = ObjectPool::instance();

        std::vector<rttr::argument> args;

        if (is_method_call())
        {
            const auto instance      = pool.get_object(instance_name_.value());
            const auto instance_type = instance.get_type();
            rttr::method method      = instance_type.get_method(function_name_);

            if (!method.is_valid())
                throw std::runtime_error("Method not found: " + function_name_ +
                                         " on type: " + instance_type.get_name().to_string());

            for (const auto& param : method.get_parameter_infos())
            {
                const auto param_name = param.get_name().to_string();
                if (parameters_.find(param_name) != parameters_.end())
                {
                    auto value = parameters_[param_name];

                    if (value.is_string())
                    {
                        std::string str_value = value.get<std::string>();
                        if (!str_value.empty() && str_value[0] == '$')
                        {
                            value = pool.resolve_reference(str_value.substr(1));
                        }
                    }

                    args.emplace_back(
                        serialization::RttrConverter::fromJson(value, param.get_type()));
                }
            }

            auto result = method.invoke_variadic(instance, args);

            if (alias_.has_value() && result.is_valid() && !result.is_type<void>())
            {
                pool.store(alias_.value(), std::move(result));
            }
        }
        else
        {
            const auto instance      = pool.get_object(instance_name_.value());
            const auto instance_type = instance.get_type();
            rttr::method method      = instance_type.get_method(function_name_);

            const auto* function_info = registry.get_method(name_);
            if (!function_info)
                throw std::runtime_error("Function not found: " + name_);

            method = function_info->method();

            for (const auto& param : method.get_parameter_infos())
            {
                const auto param_name = param.get_name().to_string();
                if (parameters_.find(param_name) != parameters_.end())
                {
                    auto value = parameters_[param_name];

                    if (value.is_string())
                    {
                        std::string str_value = value.get<std::string>();
                        if (!str_value.empty() && str_value[0] == '$')
                        {
                            value = pool.resolve_reference(str_value.substr(1));
                        }
                    }

                    args.emplace_back(
                        serialization::RttrConverter::fromJson(value, param.get_type()));
                }
            }

            auto result = method.invoke_variadic({}, args);

            if (alias_.has_value() && result.is_valid() && !result.is_type<void>())
            {
                pool.store(alias_.value(), std::move(result));
            }
        }
    }

    bool CallStep::is_method_call() const noexcept
    {
        return instance_name_.has_value();
    }

    void CallStep::parse_call_name()
    {
        const auto dot_pos = name_.find('.');
        if (dot_pos != std::string::npos)
        {
            auto instance_part = name_.substr(0, dot_pos);
            if (!instance_part.empty() && instance_part[0] == '$')
                instance_name_ = instance_part.substr(1);
            else
                instance_name_ = instance_part;

            function_name_ = name_.substr(dot_pos + 1);
        }
        else
        {
            function_name_ = name_;
        }
    }
} // namespace engine::pipeline