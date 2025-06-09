#include "pipeline/Command.hpp"

#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"

namespace engine::pipeline
{
    std::vector<std::string> ref_objects_aliases;

    Command::Command(const nlohmann::json& json)
    {
        if (json.contains("new"))
        {
            name_ = json["new"].get<std::string>();
        }
        else if (json.contains("call"))
        {
            name_ = json["call"].get<std::string>();
        }
        else
            throw std::invalid_argument("Command must contain either 'new' or 'call'");

        if (json.contains("as"))
            alias_ = json["as"].get<std::string>();

        if (json.contains("with") && json["with"].is_object())
        {
            for (const auto& [key, value] : json["with"].items())
            {
                parameters_.emplace(key, value);
            }
        }
    }

    void Command::execute()
    {
        if (is_constructor())
        {
            execute_constructor();
        }
        else if (is_method_call())
        {
            execute_method();
        }
        else
        {
            execute_global_method();
        }
    }

    void Command::execute_constructor()
    {
        const auto& registry   = reflection::ReflectionRegistry::instance();
        const auto* class_info = registry.get_class(name_);

        if (!class_info)
            throw std::runtime_error("Class not found: " + name_);

        if (!alias_.has_value())
            throw std::invalid_argument("Constructor must have 'as' field");

        rttr::variant object;

        if (!parameters_.empty())
        {
            auto constructors = class_info->type().get_constructors();
            for (const auto& ctor : constructors)
            {
                auto arguments = resolve_arguments_for_constructor(ctor);
                std::vector<rttr::argument> args(arguments.begin(), arguments.end());
                if (!args.empty())
                {
                    object = ctor.invoke_variadic(args);
                    if (object.is_valid())
                        break;
                }
            }
        }

        if (!object.is_valid())
        {
            object = class_info->type().create();
        }

        if (!object.is_valid())
            throw std::runtime_error("Failed to create instance: " + name_);

        ObjectPool::instance().store(alias_.value(), std::move(object));
    }

    void Command::execute_method()
    {
        const auto dot_pos       = name_.find('.');
        const auto instance_name = name_.substr(0, dot_pos);
        const auto method_name   = name_.substr(dot_pos + 1);

        const auto clean_instance =
            instance_name[0] == ('$') ? instance_name.substr(1) : instance_name;

        const auto& instance = ObjectPool::instance().get_object(clean_instance);
        const auto method    = instance.get_type().get_method(method_name);

        if (!method.is_valid())
            throw std::runtime_error("Method not found: " + method_name);

        auto arguments = resolve_arguments(method);
        std::vector<rttr::argument> args(arguments.begin(), arguments.end());
        auto result = method.invoke_variadic(instance, args);

        for (size_t i = 0; i < ref_objects_aliases.size(); ++i)
        {
            if (!ref_objects_aliases[i].empty())
            {
                ObjectPool::instance().update_object(ref_objects_aliases[i], arguments[i]);
            }
        }
    }

    void Command::execute_global_method()
    {
        const auto& registry    = reflection::ReflectionRegistry::instance();
        const auto* method_info = registry.get_method(name_);

        if (!method_info)
            throw std::runtime_error("Function not found: " + name_);

        auto arguments = resolve_arguments(method_info->method());
        std::vector<rttr::argument> args(arguments.begin(), arguments.end());
        auto result = method_info->method().invoke_variadic({}, args);

        for (size_t i = 0; i < ref_objects_aliases.size(); ++i)
        {
            if (!ref_objects_aliases[i].empty())
            {
                ObjectPool::instance().update_object(ref_objects_aliases[i], arguments[i]);
            }
        }
    }

    bool Command::is_constructor() const noexcept
    {
        return std::isupper(name_[0]);
    }

    bool Command::is_method_call() const noexcept
    {
        return name_.find('.') != std::string::npos;
    }

    std::vector<rttr::variant> Command::resolve_arguments(const rttr::method& method)
    {
        ref_objects_aliases.clear();
        std::vector<rttr::variant> args;
        args.reserve(method.get_parameter_infos().size());

        for (const auto& param_info : method.get_parameter_infos())
        {
            const auto param_name = param_info.get_name().to_string();

            if (parameters_.find(param_name) == parameters_.end())
                continue;

            const auto& value     = parameters_[param_name];
            const auto param_type = param_info.get_type();

            if (value.is_string())
            {
                const auto str_value = value.get<std::string>();

                if (str_value[0] == '$')
                {
                    auto obj = ObjectPool::instance().get_object(str_value.substr(1));
                    args.emplace_back(obj);

                    if (param_type.is_class())
                        ref_objects_aliases.push_back(str_value.substr(1));

                    continue;
                }
            }

            args.emplace_back(
                serialization::RttrConverter::fromJson(value, param_info.get_type()).value());
        }

        return args;
    }

    std::vector<rttr::variant>
    Command::resolve_arguments_for_constructor(const rttr::constructor& ctor)
    {
        std::vector<rttr::variant> args;
        args.reserve(ctor.get_parameter_infos().size());

        for (const auto& param_info : ctor.get_parameter_infos())
        {
            const auto param_name = param_info.get_name().to_string();

            auto it = parameters_.find(param_name);
            if (it == parameters_.end())
                continue;

            const auto& value = it->second;

            if (value.is_array() && value.get<std::string>()[0] == '$')
            {
                auto obj = ObjectPool::instance().get_object(value.get<std::string>().substr(1));
                args.emplace_back(obj);
            }
            else
            {
                auto converted =
                    serialization::RttrConverter::fromJson(value, param_info.get_type());

                if (converted.has_value())
                {
                    args.emplace_back(converted.value());
                }
            }
        }

        return args;
    }
} // namespace engine::pipeline