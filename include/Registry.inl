#ifndef REGISTRY_INL
#define REGISTRY_INL

#include "ParameterConverter.h"
#include "ResultConverter.h"

template<typename T>
void Registry::registerInstance(const std::string &name, T *instance) {
    if (instance == nullptr) {
        throw std::invalid_argument("Cannot register null instance for '" + name + "'");
    }
    instances_[name] = instance;
}

inline void *Registry::getInstance(const std::string &name) const {
    const auto it = instances_.find(name);
    if (it == instances_.end()) {
        return nullptr;
    }
    return it->second;
}

template<typename Class, typename Ret, typename... Args>
void Registry::registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                      Ret (Class::*function)(Args...)) {
    void *instancePtr = getInstance(instanceName);
    if (!instancePtr) {
        throw std::runtime_error("Instance '" + instanceName + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    instanceMethods_[instanceName][methodName] = [instance, function, methodName
            ](const std::vector<std::string> &params) {
                if (params.size() < sizeof...(Args)) {
                    throw std::invalid_argument(
                        methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
                }
                return callInstanceMethod(*instance, function, params, std::index_sequence_for<Args...>{});
            };
}

template<typename Class, typename Ret>
void Registry::registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                      Ret (Class::*function)()) {
    void *instancePtr = getInstance(instanceName);
    if (!instancePtr) {
        throw std::runtime_error("Instance '" + instanceName + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    instanceMethods_[instanceName][methodName] = [instance, function](const std::vector<std::string> &) {
        Ret result = (instance->*function)();
        return ResultConverter::convert(result);
    };
}

template<typename Class, typename Ret>
void Registry::registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                      Ret (Class::*function)() const) {
    void *instancePtr = getInstance(instanceName);
    if (!instancePtr) {
        throw std::runtime_error("Instance '" + instanceName + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    instanceMethods_[instanceName][methodName] = [instance, function](const std::vector<std::string> &) {
        Ret result = (instance->*function)();
        return ResultConverter::convert(result);
    };
}

template<typename Ret, typename... Args>
void Registry::registerGlobalMethod(const std::string &methodName, Ret (*function)(Args...)) {
    globalMethods_[methodName] = [function, methodName](const std::vector<std::string> &params) {
        if (params.size() < sizeof...(Args)) {
            throw std::invalid_argument(methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
        }
        return callGlobalMethod(function, params, std::index_sequence_for<Args...>{});
    };
}

template<typename Ret>
void Registry::registerGlobalMethod(const std::string &methodName, Ret (*function)()) {
    globalMethods_[methodName] = [function](const std::vector<std::string> &) {
        Ret result = (*function)();
        return ResultConverter::convert(result);
    };
}

template<typename Class, typename Ret, typename... Args, std::size_t... I>
std::string Registry::callInstanceMethod(Class &instance, Ret (Class::*function)(Args...),
                                         const std::vector<std::string> &params, std::index_sequence<I...>) {
    if constexpr (std::is_same_v<Ret, void>) {
        (instance.*function)(ParameterConverter::convert<Args>(params[I])...);
        return "Operation completed successfully";
    } else {
        Ret result = (instance.*function)(ParameterConverter::convert<Args>(params[I])...);
        return ResultConverter::convert(result);
    }
}

template<typename Ret, typename... Args, std::size_t... I>
std::string Registry::callGlobalMethod(Ret (*function)(Args...), const std::vector<std::string> &params,
                                       std::index_sequence<I...>) {
    if constexpr (std::is_same_v<Ret, void>) {
        (*function)(ParameterConverter::convert<Args>(params[I])...);
        return "Operation completed successfully";
    } else {
        Ret result = (*function)(ParameterConverter::convert<Args>(params[I])...);
        return ResultConverter::convert(result);
    }
}

#endif //REGISTRY_INL
