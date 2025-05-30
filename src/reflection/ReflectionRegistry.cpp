#include "reflection/ReflectionRegistry.hpp"

#include "pch.hpp"

using namespace engine::model;

namespace engine::reflection
{
    ReflectionRegistry& ReflectionRegistry::instance()
    {
        static ReflectionRegistry instance;
        return instance;
    }

    void ReflectionRegistry::register_all_from_rttr()
    {
        for (const auto& type : rttr::type::get_types())
        {
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
            methods.emplace_back(&method);
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
            return &classes_.find(std::string{name})->second;

        return nullptr;
    }

    bool ReflectionRegistry::has_method(std::string_view name) const
    {
        return methods_.find(std::string{name}) != methods_.end();
    }

    const Method* ReflectionRegistry::get_method(std::string_view name) const
    {
        if (has_method(name))
            return &methods_.find(std::string{name})->second;

        return nullptr;
    }

    // TODO: std::string to std::string_view (might dangling reference issue)
    void ReflectionRegistry::register_class(const rttr::type& type)
    {
        const auto name = type.get_name().to_string();
        const auto id   = type.get_metadata("id").to_string();

        // TODO: std::make_unique for Class
        classes_.emplace(name, Class(id, type));
    }

    void ReflectionRegistry::register_method(const rttr::method& method)
    {
        const auto name         = method.get_name().to_string();
        const auto& return_type = method.get_return_type();
        const auto category     = method.get_metadata("category").to_string();
        const auto description  = method.get_metadata("description").to_string();

        // TODO: std::make_unique for Parameter
        std::vector<Parameter> parameters;
        parameters.reserve(method.get_parameter_infos().size());

        for (const auto& parameter : method.get_parameter_infos())
        {
            const auto parameter_name = parameter.get_name().to_string();

            if (parameter.get_type().is_class())
                parameters.emplace_back(register_class_as_parameter(parameter_name));
            else
                parameters.emplace_back(parameter_name, parameter.get_type(),
                                        parameter.get_default_value());
        }

        // TODO: std::make_unique for Method
        methods_.emplace(name, Method(method, return_type, std::move(parameters),
                                      method.is_static(), category, description));
    }

    Parameter ReflectionRegistry::register_class_as_parameter(std::string_view name) const
    {
        if (!has_class(name))
            throw std::runtime_error("Class not found: " + std::string{name});

        const auto* class_ = get_class(name);

        return Parameter(class_->id(), class_->type(), rttr::variant(), true);
    }
} // namespace engine::reflection