#include "../../include/core/Engine.h"

#include <iostream>

namespace engine {

std::mutex Engine::instanceMutex;
std::unique_ptr<Engine> Engine::instancePtr;

Engine &Engine::instance() {
  std::lock_guard lock(instanceMutex);
  if (!instancePtr) {
    instancePtr = std::unique_ptr<Engine>(new Engine());
  }
  return *instancePtr;
}

void Engine::registerMethods() {
  std::lock_guard lock(methodMutex);
  methodRegistry.clear();

  auto methods = rttr::type::get_global_methods();
  for (const auto &method : methods) {
    if (auto category = method.get_metadata("category");
        category.is_valid() && category.to_string() == "engine") {
      methodRegistry.emplace(method.get_name().to_string(), method);
      std::cout << "Registered method: " << method.get_name().to_string()
                << std::endl;
    }
  }
}

std::vector<MethodDescriptor> Engine::getAvailableMethods() const {
  std::lock_guard lock(methodMutex);
  std::vector<MethodDescriptor> result;

  for (const auto &[name, method] : methodRegistry) {
    MethodDescriptor descriptor;
    descriptor.name = name;

    auto returnType = method.get_return_type();
    descriptor.returnType = returnType.get_name().to_string();

    if (auto category = method.get_metadata("category"); category.is_valid()) {
      descriptor.category = category.to_string();
    }

    auto paramInfos = method.get_parameter_infos();
    for (const auto &param : paramInfos) {
      MethodParameter parameter;
      parameter.name = param.get_name().to_string();
      parameter.type = param.get_type().get_name().to_string();

      if (auto defaultValue = param.get_default_value();
          defaultValue.is_valid()) {
        parameter.defaultValue = defaultValue.to_string();
      }

      descriptor.parameters.push_back(parameter);
    }

    result.push_back(descriptor);
  }

  return result;
}

nlohmann::json Engine::executeMethod(const std::string &methodName,
                                     const nlohmann::json &params) {
  try {
    const auto method = [this, &methodName]() -> rttr::method {
      std::lock_guard lock(methodMutex);
      const auto it = methodRegistry.find(methodName);
      if (it == methodRegistry.end()) {
        throw std::runtime_error("Method not found: " + methodName);
      }
      return it->second;
    }();

    std::vector<rttr::argument> args;
    auto paramInfos = method.get_parameter_infos();

    for (const auto &paramInfo : paramInfos) {
      const auto &paramName = paramInfo.get_name().to_string();

      if (!params.contains(paramName)) {
        if (paramInfo.has_default_value()) {
          args.emplace_back(paramInfo.get_default_value());
          continue;
        }
        return {{"error", true},
                {"message", "Missing required parameter: " + paramName}};
      }

      const auto &paramType = paramInfo.get_type();
      const auto &jsonValue = params[paramName];

      if (paramType == rttr::type::get<int>()) {
        args.emplace_back(jsonValue.get<int>());
      } else if (paramType == rttr::type::get<double>()) {
        args.emplace_back(jsonValue.get<double>());
      } else if (paramType == rttr::type::get<std::string>()) {
        args.emplace_back(jsonValue.get<std::string>());
      } else if (paramType == rttr::type::get<bool>()) {
        args.emplace_back(jsonValue.get<bool>());
      } else {
        return {{"error", true},
                {"message", "Unsupported parameter type: " +
                                paramType.get_name().to_string()}};
      }
    }

    rttr::variant result = method.invoke_variadic({}, args);

    if (!result.is_valid()) {
      return {{"error", true}, {"message", "Method execution failed"}};
    }

    if (result.get_type() == rttr::type::get<nlohmann::json>()) {
      return result.get_value<nlohmann::json>();
    }
    if (result.get_type() == rttr::type::get<std::string>()) {
      return result.get_value<std::string>();
    }
    if (result.get_type() == rttr::type::get<int>()) {
      return result.get_value<int>();
    }
    if (result.get_type() == rttr::type::get<double>()) {
      return result.get_value<double>();
    }
    if (result.get_type() == rttr::type::get<bool>()) {
      return result.get_value<bool>();
    }
    return {{"result", "Result type not directly convertible to JSON"}};
  } catch (const std::exception &e) {
    return {{"error", true},
            {"message",
             std::string("Exception during method execution: ") + e.what()}};
  }
}

} // namespace engine