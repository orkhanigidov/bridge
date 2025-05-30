#include "pipeline/CallStep.hpp"

#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"
#include "pipeline/Step.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"

namespace engine::pipeline
{
    CallStep::CallStep(const nlohmann::json& json) : Step(json)
    {
        parse_call_name();
    }

    void CallStep::execute()
    {
        const auto& registry = reflection::ReflectionRegistry::instance();
        auto& pool           = ObjectPool::instance();

        std::vector<rttr::variant> args;
        std::vector<std::string> ref_objects_ids;

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

                        if (str_value.empty())
                        {
                            args.emplace_back(nullptr);
                            ref_objects_ids.push_back("");
                            continue;
                        }

                        if (!str_value.empty() && str_value[0] == '$')
                        {
                            auto& resolved = pool.get_object(str_value.substr(1));
                            args.emplace_back(resolved);

                            if (param.get_type().is_class())
                            {
                                ref_objects_ids.push_back(str_value.substr(1));
                            }
                            else
                            {
                                ref_objects_ids.push_back("");
                            }
                            continue;
                        }
                    }

                    args.emplace_back(
                        serialization::RttrConverter::fromJson(value, param.get_type()).value());
                    ref_objects_ids.push_back("");
                }
            }

            const std::vector<rttr::argument> params(args.begin(), args.end());

            auto result = method.invoke_variadic(instance, params);

            for (size_t i = 0; i < ref_objects_ids.size(); ++i)
            {
                if (!ref_objects_ids[i].empty())
                {
                    pool.update_object(ref_objects_ids[i], args[i]);
                }
            }

            if (result.is_valid() && !result.is_type<void>())
                std::cout << result.get_value<bool>();

            if (alias_.has_value() && result.is_valid() && !result.is_type<void>())
            {
                pool.store(alias_.value(), std::move(result));
            }
        }
        else
        {
            const auto* function_info = registry.get_method(name_);
            if (!function_info)
                throw std::runtime_error("Function not found: " + name_);

            rttr::method method = function_info->method();

            for (const auto& param : method.get_parameter_infos())
            {
                const auto param_name = param.get_name().to_string();
                if (parameters_.find(param_name) != parameters_.end())
                {
                    auto value = parameters_[param_name];

                    if (value.is_string())
                    {
                        std::string str_value = value.get<std::string>();

                        if (str_value.empty())
                        {
                            args.emplace_back(nullptr);
                            ref_objects_ids.push_back("");
                            continue;
                        }

                        if (!str_value.empty() && str_value[0] == '$')
                        {
                            auto& resolved = pool.get_object(str_value.substr(1));
                            args.emplace_back(resolved);

                            if (param.get_type().is_class())
                            {
                                ref_objects_ids.push_back(str_value.substr(1));
                            }
                            else
                            {
                                ref_objects_ids.push_back("");
                            }
                            continue;
                        }
                    }

                    args.emplace_back(
                        serialization::RttrConverter::fromJson(value, param.get_type()).value());
                }
            }

            const std::vector<rttr::argument> params(args.begin(), args.end());

            auto result = rttr::variant();
            if (method.get_name() == "write")
            {
                result = method.invoke({}, params[0], params[1], nullptr);
            }
            else
            {
                result = method.invoke_variadic({}, params);
            }

            for (size_t i = 0; i < ref_objects_ids.size(); ++i)
            {
                if (!ref_objects_ids[i].empty())
                {
                    pool.update_object(ref_objects_ids[i], args[i]);
                }
            }

            if (result.is_valid() && !result.is_type<void>())
                std::cout << result.get_value<bool>();

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