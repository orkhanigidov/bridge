#ifndef REGISTRY_INL
#define REGISTRY_INL

#include "ParameterConverter.h"
#include "ResultConverter.h"

template<typename T>
void Registry::registerObject(const std::string &objectId, T *instance) {
    if (instance == nullptr) {
        throw std::invalid_argument("Cannot register null instance for '" + objectId + "'");
    }
    objectInstances_[objectId] = instance;
}

inline void *Registry::getInstance(const std::string &objectId) const {
    const auto it = objectInstances_.find(objectId);
    if (it == objectInstances_.end()) {
        return nullptr;
    }
    return it->second;
}

template<typename Class, typename Ret, typename... Args>
void Registry::registerMemberMethod(const std::string &objectId, const std::string &methodId,
                                    Ret (Class::*methodPtr)(Args...)) {
    void *instancePtr = getInstance(objectId);
    if (!instancePtr) {
        throw std::runtime_error("Object '" + objectId + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    memberMethods_[objectId][methodId] = [instance, methodPtr, methodId
            ](const std::vector<std::string> &args) {
                if (args.size() < sizeof...(Args)) {
                    throw std::invalid_argument(
                        methodId + " requires " + std::to_string(sizeof...(Args)) + " arguments");
                }
                return invokeMemberMethod(*instance, methodPtr, args, std::index_sequence_for<Args...>{});
            };
}

template<typename Class, typename Ret>
void Registry::registerMemberMethod(const std::string &objectId, const std::string &methodId,
                                    Ret (Class::*methodPtr)()) {
    void *instancePtr = getInstance(objectId);
    if (!instancePtr) {
        throw std::runtime_error("Object '" + objectId + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    memberMethods_[objectId][methodId] = [instance, methodPtr](const std::vector<std::string> &) {
        Ret result = (instance->*methodPtr)();
        return ResultConverter::convert(result);
    };
}

template<typename Class, typename Ret>
void Registry::registerMemberMethod(const std::string &objectId, const std::string &methodId,
                                    Ret (Class::*methodPtr)() const) {
    void *instancePtr = getInstance(objectId);
    if (!instancePtr) {
        throw std::runtime_error("Object '" + objectId + "' not found");
    }
    auto *instance = static_cast<Class *>(instancePtr);

    memberMethods_[objectId][methodId] = [instance, methodPtr](const std::vector<std::string> &) {
        Ret result = (instance->*methodPtr)();
        return ResultConverter::convert(result);
    };
}

template<typename Ret, typename... Args>
void Registry::registerGlobalMethod(const std::string &methodId, Ret (*functionPtr)(Args...)) {
    globalMethods_[methodId] = [functionPtr, methodId](const std::vector<std::string> &args) {
        if (args.size() < sizeof...(Args)) {
            throw std::invalid_argument(methodId + " requires " + std::to_string(sizeof...(Args)) + " arguments");
        }
        return invokeGlobalMethod(functionPtr, args, std::index_sequence_for<Args...>{});
    };
}

template<typename Ret>
void Registry::registerGlobalMethod(const std::string &methodId, Ret (*functionPtr)()) {
    globalMethods_[methodId] = [functionPtr](const std::vector<std::string> &) {
        Ret result = (*functionPtr)();
        return ResultConverter::convert(result);
    };
}

template<typename Class, typename Ret, typename... Args, std::size_t... I>
std::string Registry::invokeMemberMethod(Class &instance, Ret (Class::*methodPtr)(Args...),
                                         const std::vector<std::string> &args, std::index_sequence<I...>) {
    if constexpr (std::is_same_v<Ret, void>) {
        (instance.*methodPtr)(ParameterConverter::convert<Args>(args[I])...);
        return "Operation completed successfully";
    } else {
        Ret result = (instance.*methodPtr)(ParameterConverter::convert<Args>(args[I])...);
        return ResultConverter::convert(result);
    }
}

template<typename Ret, typename... Args, std::size_t... I>
std::string Registry::invokeGlobalMethod(Ret (*functionPtr)(Args...), const std::vector<std::string> &args,
                                         std::index_sequence<I...>) {
    if constexpr (std::is_same_v<Ret, void>) {
        (*functionPtr)(ParameterConverter::convert<Args>(args[I])...);
        return "Operation completed successfully";
    } else {
        Ret result = (*functionPtr)(ParameterConverter::convert<Args>(args[I])...);
        return ResultConverter::convert(result);
    }
}

#endif //REGISTRY_INL
