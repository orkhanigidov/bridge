#include "../../include/core/Engine.h"

#include "../../include/core/TypeConverter.h"
#include <iostream>

namespace engine {

    std::mutex Engine::instanceMutex;
    std::unique_ptr<Engine> Engine::instancePtr;

    Engine& Engine::instance() {
        std::lock_guard lock(instanceMutex);
        if (!instancePtr) {
            instancePtr = std::unique_ptr<Engine>(new Engine());
        }
        return *instancePtr;
    }

    void Engine::registerMethods() {
        std::lock_guard lock(methodMutex);
        methodRegistry.clear();

        for (const auto& method : rttr::type::get_global_methods()) {
            // if (auto category = method.get_metadata("category");
            // category.is_valid() && category.to_string() == "engine") {
            const std::string methodName = method.get_name().to_string();
            methodRegistry.emplace(methodName, method);
            std::cout << "Registered method: " << methodName << std::endl;
            // }
        }
    }

    std::vector<Method> Engine::getAvailableMethods() const {
        std::lock_guard lock(methodMutex);
        std::vector<Method> result;

        for (const auto& [name, method] : methodRegistry) {
            Method descriptor;
            descriptor.name       = name;
            descriptor.returnType = method.get_return_type().get_name().to_string();

            if (auto category = method.get_metadata("category"); category.is_valid()) {
                descriptor.category = category.to_string();
            }

            if (auto desc = method.get_metadata("description"); desc.is_valid()) {
                descriptor.description = desc.to_string();
            }

            for (const auto& param : method.get_parameter_infos()) {
                MethodParameter parameter;
                parameter.name = param.get_name().to_string();
                parameter.type = param.get_type().get_name().to_string();

                if (auto defaultValue = param.get_default_value(); defaultValue.is_valid()) {
                    parameter.defaultValue = defaultValue.to_string();
                }
                descriptor.parameters.push_back(parameter);
            }
            result.push_back(descriptor);
        }

        return result;
    }

    nlohmann::json Engine::executeMethod(const std::string& methodName, const nlohmann::json& params) {
        try {
            std::lock_guard lock(methodMutex);
            const auto it = methodRegistry.find(methodName);
            if (it == methodRegistry.end()) {
                return createErrorResponse("Method not found: " + methodName);
            }
            const rttr::method& method = it->second;

            std::vector<rttr::argument> args;
            for (const auto& paramInfo : method.get_parameter_infos()) {
                const std::string paramName = paramInfo.get_name().to_string();
                const rttr::type paramType  = paramInfo.get_type();

                if (!params.contains(paramName)) {
                    if (paramInfo.has_default_value()) {
                        args.emplace_back(paramInfo.get_default_value());
                        continue;
                    }
                    return createErrorResponse("Missing required parameter: " + paramName);
                }

                const auto& jsonValue = params[paramName];
                auto [success, arg]   = jsonToRttrArgument(jsonValue, paramType);
                if (!success) {
                    return createErrorResponse("Type mismatch for parameter: " + paramName
                                               + " or unsupported type: " + paramType.get_name().to_string());
                }
                args.push_back(arg);
            }

            rttr::variant result = method.invoke_variadic({}, args);
            return rttrVariantToJson(result);
        } catch (const std::exception& e) {
            return createErrorResponse("Exception: " + std::string(e.what()));
        }
    }

} // namespace engine
