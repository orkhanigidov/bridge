#include "reflection/ReflectionRegistry.hpp"

#include "pch.hpp"

using namespace engine::model;

namespace engine::reflection
{
    void ReflectionRegistry::register_all_from_rttr()
    {
        for (const auto& type : rttr::type::get_types())
        {
            if (type.get_metadata("alias").is_valid())
                register_class(type);
        }

        for (const auto& type : rttr::type::get_types())
        {
            for (const auto& method : type.get_methods())
            {
                register_method(method);
            }
        }

        for (const auto& method : rttr::type::get_global_methods())
        {
            register_method(method);
        }
    }

    std::vector<const Method*> ReflectionRegistry::get_registered_methods() const
    {
        std::vector<const Method*> methods;
        methods.reserve(methods_.size());

        for (const auto& [key, method] : methods_)
        {
            methods.emplace_back(method.get());
        }

        return methods;
    }

    bool ReflectionRegistry::has_class(std::string_view name) const
    {
        return classes_.find(std::string{name}) != classes_.end();
    }

    const Class* ReflectionRegistry::get_class(std::string_view name) const
    {
        if (has_class(name))
            return classes_.find(std::string{name})->second.get();

        return nullptr;
    }

    bool ReflectionRegistry::has_method(std::string_view name) const
    {
        return methods_.find(std::string{name}) != methods_.end();
    }

    const Method* ReflectionRegistry::get_method(std::string_view name) const
    {
        if (has_method(name))
            return methods_.find(std::string{name})->second.get();

        return nullptr;
    }

    void ReflectionRegistry::register_class(const rttr::type& type)
    {
        const auto name  = type.get_name().to_string();
        const auto alias = type.get_metadata("alias").to_string();

        classes_.emplace(name, std::make_unique<Class>(alias, type));
    }

    void ReflectionRegistry::register_method(const rttr::method& method)
    {
        const auto name         = method.get_name().to_string();
        const auto& return_type = method.get_return_type();
        const auto category     = method.get_metadata("category").to_string();
        const auto description  = method.get_metadata("description").to_string();

        std::vector<Parameter> parameters;
        parameters.reserve(method.get_parameter_infos().size());

        for (const auto& parameter : method.get_parameter_infos())
        {
            parameters.emplace_back(parameter.get_name().to_string(), parameter.get_type(),
                                    parameter.get_default_value());
        }

        methods_.emplace(name, std::make_unique<Method>(method, return_type, std::move(parameters),
                                                        method.is_static(), category, description));
    }
} // namespace engine::reflection