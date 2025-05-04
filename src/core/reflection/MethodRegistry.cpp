#include "../../../include/pch.h"

#include "../../../include/core/reflection/MethodRegistry.h"

namespace engine::core::reflection
{

std::mutex MethodRegistry::instanceMutex;
std::unique_ptr<MethodRegistry> MethodRegistry::instancePtr;

MethodRegistry &MethodRegistry::getInstance()
{
    std::lock_guard lock(instanceMutex);
    if (!instancePtr)
    {
        instancePtr = std::unique_ptr<MethodRegistry>(new MethodRegistry());
    }
    return *instancePtr;
}

void MethodRegistry::registerMethod(const rttr::method &method, const std::string &methodName)
{
    const auto category = method.get_metadata("category");
    if (!category.is_valid())
    {
        return;
    }

    model::MethodDescriptor descriptor;
    descriptor.name = methodName;
    descriptor.category = category.to_string();
    descriptor.returnType = method.get_return_type().get_name().to_string();

    if (const auto desc = method.get_metadata("description"); desc.is_valid())
    {
        descriptor.description = desc.to_string();
    }

    for (const auto &param : method.get_parameter_infos())
    {
        model::MethodParameterDescriptor parameter;
        parameter.name = param.get_name().to_string();
        parameter.type = param.get_type().get_name().to_string();

        if (auto defaultValue = param.get_default_value(); defaultValue.is_valid())
        {
            parameter.defaultValue = defaultValue.to_string();
        }

        descriptor.parameters.push_back(parameter);
    }

    methodMap.emplace(methodName, std::move(descriptor));
    std::cout << "Registered method " << methodName << std::endl;
}

void MethodRegistry::registerAll()
{
    std::lock_guard lock(methodMutex);
    methodMap.clear();

    for (const auto &method : rttr::type::get_global_methods())
    {
        registerMethod(method, method.get_name().to_string());
    }

    for (const auto &type : rttr::type::get_types())
    {
        for (const auto &method : type.get_methods())
        {
            registerMethod(method, type.get_name().to_string() + "::" + method.get_name().to_string());
        }
    }
}

std::vector<model::MethodDescriptor> MethodRegistry::getRegisteredMethods() const
{
    std::lock_guard lock(methodMutex);
    std::vector<model::MethodDescriptor> result;
    result.reserve(methodMap.size());

    for (const auto &[name, descriptor] : methodMap)
    {
        result.push_back(descriptor);
    }

    return result;
}

bool MethodRegistry::hasMethod(const std::string &methodName) const
{
    std::lock_guard lock(methodMutex);
    return methodMap.find(methodName) != methodMap.end();
}

model::MethodDescriptor MethodRegistry::findMethod(const std::string &methodName) const
{
    std::lock_guard lock(methodMutex);
    if (const auto it = methodMap.find(methodName); it != methodMap.end())
    {
        return it->second;
    }
    return {};
}

} // namespace engine::core::reflection