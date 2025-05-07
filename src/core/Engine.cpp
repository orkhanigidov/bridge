#include "../../include/pch.h"

#include "../../include/core/Engine.h"
#include "../../include/core/reflection/MethodRegistry.h"
#include "../../include/serialization/JsonRttrConverter.h"

using namespace rttr;

namespace engine::core
{

std::mutex Engine::instanceMutex;
std::unique_ptr<Engine> Engine::instancePtr;

Engine &Engine::getInstance()
{
    std::lock_guard lock(instanceMutex);
    if (!instancePtr)
    {
        instancePtr = std::unique_ptr<Engine>(new Engine());
    }
    return *instancePtr;
}

nlohmann::json Engine::executeMethod(const std::string &methodName, const nlohmann::json &params)
{
    const auto &methodRegistry = reflection::MethodRegistry::getInstance();
    if (!methodRegistry.hasMethod(methodName))
    {
        return serialization::JsonRttrConverter::errorToJson("Method not found: " + methodName);
    }

    try
    {
        const model::MethodDescriptor descriptor = methodRegistry.findMethod(methodName);
        const method method = type::get_global_method(descriptor.name);

        if (!method.is_valid())
        {
            return serialization::JsonRttrConverter::errorToJson("Invalid RTTR method: " + methodName);
        }

        const std::vector<variant> convertedParams =
            serialization::JsonRttrConverter::convertMethodParams(method, params);
        const std::vector<argument> args(convertedParams.begin(), convertedParams.end());
        const variant returnValue = method.invoke_variadic({}, args);

        if (!returnValue.is_valid() && method.get_return_type() != type::get<void>())
        {
            return serialization::JsonRttrConverter::errorToJson("Failed to invoke method: " + methodName);
        }

        return {{"success", true}, {"result", serialization::JsonRttrConverter::variantToJson(returnValue)}};
    }
    catch (const std::exception &e)
    {
        return serialization::JsonRttrConverter::errorToJson("Exception: " + std::string(e.what()));
    }
}

} // namespace engine::core