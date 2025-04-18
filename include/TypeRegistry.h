#ifndef TYPEREGISTRY_H
#define TYPEREGISTRY_H

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include <mutex>

class TypeRegistry {
public:
    template<typename T>
    std::string registerObject(const T &obj) {
        std::lock_guard lock(mutex_);
        std::string id = generateId<T>();
        objects_[std::type_index(typeid(T))][id] = std::any(std::reference_wrapper<const T>(obj));
        return id;
    }

    template<typename T>
    T getObject(const std::string &id) const {
        std::lock_guard lock(mutex_);
        const auto &typeMap = objects_.find(std::type_index(typeid(T)));
        if (typeMap == objects_.end()) {
            throw std::runtime_error("No objects of type: " + std::string(typeid(T).name()) + " registered");
        }

        const auto &it = typeMap->second.find(id);
        if (it == typeMap->second.end()) {
            throw std::runtime_error("Object not found: " + id);
        }

        return std::any_cast<std::reference_wrapper<const T> >(it->second).get();
    }

    template<typename T>
    bool hasObject(const std::string &id) const {
        std::lock_guard lock(mutex_);
        const auto &typeMap = objects_.find(std::type_index(typeid(T)));
        if (typeMap == objects_.end()) return false;
        return typeMap->second.find(id) != typeMap->second.end();
    }

private:
    template<typename T>
    static std::string generateId() {
        static int counter = 0;
        return std::string(typeid(T).name()) + "_" + std::to_string(counter++);
    }

    mutable std::mutex mutex_;
    std::unordered_map<std::type_index, std::unordered_map<std::string, std::any> > objects_;
};

extern TypeRegistry g_typeRegistry;

#endif //TYPEREGISTRY_H
