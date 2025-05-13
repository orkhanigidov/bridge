#include "../../include/pch.h"

#include "../../include/core/Engine.h"
#include "../../include/core/reflection/MethodRegistry.h"
#include "../../include/serialization/JsonRttrConverter.h"
#include "../../include/serialization/JsonSerializer.h"

using namespace rttr;

namespace engine::core
{

std::mutex Engine::s_instanceMutex;
std::unique_ptr<Engine> Engine::s_instancePtr;

Engine &Engine::getInstance()
{
    std::lock_guard lock(s_instanceMutex);
    if (!s_instancePtr)
    {
        s_instancePtr = std::unique_ptr<Engine>(new Engine());
    }
    return *s_instancePtr;
}

nlohmann::json Engine::executeMethod(const std::string &methodName, const nlohmann::json &params)
{
    std::lock_guard lock(s_instanceMutex);

    try
    {
        const auto &registry = reflection::MethodRegistry::getInstance();
        if (!registry.hasMethod(methodName))
        {
            return serialization::JsonRttrConverter::errorToJson("Method not found: " + methodName);
        }

        const auto methodDescriptor = registry.findMethod(methodName);
        const auto method = methodDescriptor.getMethod();

        if (!method.is_valid())
        {
            return serialization::JsonRttrConverter::errorToJson("Invalid method: " + methodName);
        }

        const std::vector<variant> convertedParams =
            serialization::JsonRttrConverter::convertMethodParams(method, params);
        const std::vector<argument> args(convertedParams.begin(), convertedParams.end());

        variant result;

        if (method.is_static())
        {
            result = method.invoke_variadic({}, args);
        }
        else
        {
            const auto declaringType = method.get_declaring_type();
            if (!declaringType.is_valid())
            {
                return serialization::JsonRttrConverter::errorToJson("Invalid declaring type for method: " +
                                                                     methodName);
            }

            const auto object = declaringType.create();
            if (!object.is_valid())
            {
                return serialization::JsonRttrConverter::errorToJson("Failed to create instance of type: " +
                                                                     declaringType.get_name().to_string());
            }

            result = method.invoke_variadic(object, args);
        }

        if (!result.is_valid() && method.get_return_type() != type::get<void>())
        {
            return serialization::JsonRttrConverter::errorToJson("Method invocation failed");
        }

        return {{"success", true}, {"result", serialization::JsonRttrConverter::variantToJson(result)}};
    }
    catch (const std::exception &e)
    {
        return serialization::JsonRttrConverter::errorToJson("Exception: " + std::string(e.what()));
    }
}

nlohmann::json Engine::getRegisteredMethods()
{
    std::lock_guard lock(s_instanceMutex);

    try
    {
        const auto &registry = reflection::MethodRegistry::getInstance();
        const auto methods = registry.getAllMethods();

        nlohmann::json result = nlohmann::json::array();
        for (const auto &method : methods)
        {
            result.push_back(serialization::JsonSerializer::serialize(method));
        }

        return {{"success", true}, {"methods", result}};
    }
    catch (const std::exception &e)
    {
        return serialization::JsonRttrConverter::errorToJson("Exception: " + std::string(e.what()));
    }
}

} // namespace engine::core