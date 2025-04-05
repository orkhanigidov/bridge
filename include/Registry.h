#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Registry {
public:
    Registry();

    template<typename T>
    void registerInstance(const std::string &name, T *instance);

    inline void *getInstance(const std::string &name) const;

    template<typename Class, typename Ret, typename... Args>
    void registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                Ret (Class::*function)(Args...));

    template<typename Class, typename Ret>
    void registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                Ret (Class::*function)());

    template<typename Class, typename Ret>
    void registerInstanceMethod(const std::string &instanceName, const std::string &methodName,
                                Ret (Class::*function)() const);

    template<typename Ret, typename... Args>
    void registerGlobalMethod(const std::string &methodName, Ret (*function)(Args...));

    template<typename Ret>
    void registerGlobalMethod(const std::string &methodName, Ret (*function)());

    std::string execute(const std::string &instanceName, const std::string &methodName,
                        const std::vector<std::string> &params);

    std::string listAllMethods() const;

private:
    template<typename Class, typename Ret, typename... Args, std::size_t... I>
    static std::string callInstanceMethod(Class &instance, Ret (Class::*function)(Args...),
                                          const std::vector<std::string> &params, std::index_sequence<I...>);

    template<typename Ret, typename... Args, std::size_t... I>
    static std::string callGlobalMethod(Ret (*function)(Args...), const std::vector<std::string> &params,
                                        std::index_sequence<I...>);

    void registerInstances();

    void registerAllMethods();

    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)> > globalMethods_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::function<std::string(
        const std::vector<std::string> &)> > > instanceMethods_;
    std::unordered_map<std::string, void *> instances_;
};

#include "Registry.inl"

#endif //REGISTRY_H
