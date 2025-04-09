#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Registry {
public:
    Registry();

    ~Registry() = default;

    template<typename T>
    void registerObject(const std::string &objectId, T *instance);

    inline void *getInstance(const std::string &objectId) const;

    template<typename Class, typename Ret, typename... Args>
    void registerMemberMethod(const std::string &objectId, const std::string &methodId,
                              Ret (Class::*methodPtr)(Args...));

    template<typename Class, typename Ret>
    void registerMemberMethod(const std::string &objectId, const std::string &methodId,
                              Ret (Class::*methodPtr)());

    template<typename Class, typename Ret>
    void registerMemberMethod(const std::string &objectId, const std::string &methodId,
                              Ret (Class::*methodPtr)() const);

    template<typename Ret, typename... Args>
    void registerGlobalMethod(const std::string &methodId, Ret (*functionPtr)(Args...));

    template<typename Ret>
    void registerGlobalMethod(const std::string &methodId, Ret (*functionPtr)());

    std::string invokeMethod(const std::string &objectId, const std::string &methodId,
                             const std::vector<std::string> &args);

    std::string listAvailableMethods() const;

private:
    template<typename Class, typename Ret, typename... Args, std::size_t... I>
    static std::string invokeMemberMethod(Class &instance, Ret (Class::*methodPtr)(Args...),
                                          const std::vector<std::string> &args, std::index_sequence<I...>);

    template<typename Ret, typename... Args, std::size_t... I>
    static std::string invokeGlobalMethod(Ret (*functionPtr)(Args...), const std::vector<std::string> &args,
                                          std::index_sequence<I...>);

    void initializeObjects();

    void initializeMethods();

    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)> > globalMethods_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::function<std::string(
        const std::vector<std::string> &)> > > memberMethods_;
    std::unordered_map<std::string, void *> objectInstances_;
};

#include "Registry.inl"

#endif //REGISTRY_H
