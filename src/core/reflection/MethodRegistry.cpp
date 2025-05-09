#include "../../../include/pch.h"

#include "../../../include/core/reflection/MethodRegistry.h"

namespace engine::core::reflection
{

std::mutex MethodRegistry::s_instanceMutex;
std::unique_ptr<MethodRegistry> MethodRegistry::s_instancePtr;

MethodRegistry &MethodRegistry::getInstance()
{
    std::lock_guard lock(s_instanceMutex);
    if (!s_instancePtr)
    {
        s_instancePtr = std::unique_ptr<MethodRegistry>(new MethodRegistry());
    }
    return *s_instancePtr;
}

void MethodRegistry::registerMethod(const rttr::method &method)
{
    const auto methodName = method.get_name();
    const auto category = method.get_metadata("category").to_string();
    const auto description = method.get_metadata("description").to_string();

    std::vector<model::MethodParameterDescriptor> parameters;
    parameters.reserve(method.get_parameter_infos().size());

    for (const auto &param : method.get_parameter_infos())
    {
        parameters.emplace_back(param.get_name(), param.get_type(), param.get_default_value());
    }

    auto descriptor = model::MethodDescriptor(method, method.get_return_type(), parameters, category, description);

    std::lock_guard lock(s_instanceMutex);
    m_methodMap.emplace(methodName, std::move(descriptor));
    std::cout << "Registered method " << methodName << std::endl;
}

void MethodRegistry::registerAll()
{
    std::lock_guard lock(m_methodMutex);
    m_methodMap.clear();

    for (const auto &method : rttr::type::get_global_methods())
    {
        registerMethod(method);
    }

    for (const auto &type : rttr::type::get_types())
    {
        for (const auto &method : type.get_methods())
        {
            registerMethod(method);
        }
    }
}

std::vector<model::MethodDescriptor> MethodRegistry::getRegisteredMethods() const
{
    std::lock_guard lock(m_methodMutex);
    std::vector<model::MethodDescriptor> result;
    result.reserve(m_methodMap.size());

    for (const auto &[name, descriptor] : m_methodMap)
    {
        result.push_back(descriptor);
    }

    return result;
}

bool MethodRegistry::hasMethod(const std::string &methodName) const
{
    std::lock_guard lock(m_methodMutex);
    return m_methodMap.find(methodName) != m_methodMap.end();
}

model::MethodDescriptor MethodRegistry::findMethod(const std::string &methodName) const
{
    std::lock_guard lock(m_methodMutex);
    if (const auto it = m_methodMap.find(methodName); it != m_methodMap.end())
    {
        return it->second;
    }

    return model::MethodDescriptor(rttr::type::get_global_method(""), rttr::type::get<void>(), {}, "", "");
}

} // namespace engine::core::reflection