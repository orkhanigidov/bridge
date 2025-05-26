#include "../../include/reflection/ReflectionRegistry.h"

#include "../../include/pch.h"

namespace engine::reflection
{
    ReflectionRegistry& ReflectionRegistry::getInstance()
    {
        static ReflectionRegistry instance;
        return instance;
    }

    void ReflectionRegistry::registerAllFromRTTR()
    {
        for (const auto& type : rttr::type::get_types())
        {
            registerClass(type);
        }

        for (const auto& type : rttr::type::get_types())
        {
            for (const auto& method : type.get_methods())
            {
                registerMethod(method);
            }
        }

        for (const auto& method : rttr::type::get_global_methods())
        {
            registerMethod(method);
        }
    }

    std::vector<const model::Method*> ReflectionRegistry::getAllMethods() const
    {
        std::vector<const model::Method*> result;
        result.reserve(m_methods.size());

        for (const auto& [key, method] : m_methods)
        {
            result.emplace_back(&method);
        }

        return result;
    }

    const model::Class* ReflectionRegistry::getClass(const std::string& name) const
    {
        if (hasClass(name))
            return &m_classes.find(name)->second;

        return nullptr;
    }

    const model::Method* ReflectionRegistry::getMethod(const std::string& name) const
    {
        if (hasMethod(name))
            return &m_methods.find(name)->second;

        return nullptr;
    }

    bool ReflectionRegistry::hasClass(const std::string& name) const
    {
        return m_classes.find(name) != m_classes.end();
    }

    bool ReflectionRegistry::hasMethod(const std::string& name) const
    {
        return m_methods.find(name) != m_methods.end();
    }

    void ReflectionRegistry::registerClass(const rttr::type& type)
    {
        const std::string name = type.get_name().to_string();
        const std::string id   = type.get_metadata("id").to_string();

        m_classes.emplace(name, model::Class(id, type));
    }

    void ReflectionRegistry::registerMethod(const rttr::method& method)
    {
        const std::string name        = method.get_name().to_string();
        const rttr::type returnType   = method.get_return_type();
        const std::string category    = method.get_metadata("category").to_string();
        const std::string description = method.get_metadata("description").to_string();

        std::vector<model::Parameter> parameters;
        parameters.reserve(method.get_parameter_infos().size());

        for (const auto& parameter : method.get_parameter_infos())
        {
            if (parameter.get_type().is_class())
            {
                const std::string className = parameter.get_name().to_string();
                parameters.emplace_back(registerClassAsParameter(className));
            }
            else
                parameters.emplace_back(parameter.get_name().to_string(), parameter.get_type(),
                                        parameter.get_default_value(), false);
        }

        m_methods.emplace(name, std::move(model::Method(method, returnType, parameters, category, description)));
    }

    model::Parameter ReflectionRegistry::registerClassAsParameter(const std::string& name) const
    {
        if (!hasClass(name))
            throw std::runtime_error("Class not found: " + name);

        const model::Class* type = getClass(name);
        const std::string id     = type->getId();

        return model::Parameter(id, type->getType(), nullptr, true);
    }
} // namespace engine::reflection