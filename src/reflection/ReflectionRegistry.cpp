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
            registerType(type);
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

    std::vector<model::Method> ReflectionRegistry::getAllMethods() const
    {
        std::vector<model::Method> result;
        result.reserve(m_methods.size());

        for (const auto& [key, method] : m_methods)
        {
            result.push_back(method);
        }

        return result;
    }

    const model::Type* ReflectionRegistry::getType(const std::string& name) const
    {
        if (hasType(name))
            return &m_types.find(name)->second;

        return nullptr;
    }

    const model::Method* ReflectionRegistry::getMethod(const std::string& name) const
    {
        if (hasMethod(name))
            return &m_methods.find(name)->second;

        return nullptr;
    }

    bool ReflectionRegistry::hasType(const std::string& name) const
    {
        return m_types.find(name) != m_types.end();
    }

    bool ReflectionRegistry::hasMethod(const std::string& name) const
    {
        return m_methods.find(name) != m_methods.end();
    }

    void ReflectionRegistry::registerType(const rttr::type& type)
    {
        std::string name     = type.get_name().to_string();
        const std::string id = type.get_metadata("id").to_string();

        m_types.emplace(name, model::Type(id, name));
    }

    void ReflectionRegistry::registerMethod(const rttr::method& method)
    {
        std::string name              = method.get_name().to_string();
        const rttr::type returnType   = method.get_return_type();
        const std::string category    = method.get_metadata("category").to_string();
        const std::string description = method.get_metadata("description").to_string();

        std::vector<model::Parameter> parameters;
        parameters.reserve(method.get_parameter_infos().size());

        for (const auto& parameter : method.get_parameter_infos())
        {
            parameters.emplace_back(parameter.get_name().to_string(), parameter.get_type(),
                                    parameter.get_default_value());
        }

        m_methods.emplace(
            name, std::move(model::Method(name, returnType, parameters, category, description)));
    }
} // namespace engine::reflection