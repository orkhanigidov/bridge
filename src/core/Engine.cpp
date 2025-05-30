#include "core/Engine.hpp"

#include "pch.hpp"
#include "reflection/MethodRegistrar.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/MethodSerializer.hpp"
#include "serialization/RttrConverter.hpp"

using namespace rttr;

namespace engine::core
{

    std::mutex Engine::s_instanceMutex;
    std::unique_ptr<Engine> Engine::s_instancePtr;

    Engine& Engine::getInstance()
    {
        std::lock_guard lock(s_instanceMutex);
        if (!s_instancePtr)
        {
            s_instancePtr = std::unique_ptr<Engine>(new Engine());
        }
        return *s_instancePtr;
    }

    nlohmann::json Engine::executeMethod(const std::string& methodName,
                                         const nlohmann::json& params)
    {
        std::lock_guard lock(s_instanceMutex);

        try
        {
            const auto& registry = reflection::ReflectionRegistry::instance();
            if (!registry.has_method(methodName))
            {
                throw std::runtime_error("Method not found: " + methodName);
            }

            const auto method = registry.get_method(methodName);

            if (!method->method().is_valid())
            {
                throw std::runtime_error("Method is not valid: " + methodName);
            }

            const std::vector<variant> convertedParams =
                serialization::RttrConverter::prepareMethodArguments(*method, params);
            const std::vector<argument> args(convertedParams.begin(), convertedParams.end());

            variant result;

            if (method->method().is_static())
            {
                result = method->method().invoke_variadic({}, args);
            }
            else
            {
                const auto declaringType = method->method().get_declaring_type();
                if (!declaringType.is_valid())
                {
                    throw std::runtime_error("Invalid declaring type for method: " + methodName);
                }

                const auto object = declaringType.create();
                if (!object.is_valid())
                {
                    throw std::runtime_error(
                        "Failed to create instance of declaring type for method: " + methodName);
                }

                result = method->method().invoke_variadic(object, args);
            }

            if (!result.is_valid() && method->method().get_return_type() != type::get<void>())
            {
                throw std::runtime_error(
                    "Method invocation returned an invalid result for method: " + methodName);
            }

            return {{"success", true}, {"result", serialization::RttrConverter::toJson(result)}};
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to execute method '" + methodName + "': " + e.what());
        }
    }

    nlohmann::json Engine::getRegisteredMethods()
    {
        std::lock_guard lock(s_instanceMutex);

        try
        {
            const auto& registry = reflection::ReflectionRegistry::instance();
            const auto methods   = registry.get_registered_methods();

            nlohmann::json result = nlohmann::json::array();
            for (const auto& method : methods)
            {
                result.push_back(serialization::MethodSerializer::toJson(*method));
            }

            return {{"success", true}, {"methods", result}};
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to retrieve registered methods: " +
                                     std::string(e.what()));
        }
    }

} // namespace engine::core