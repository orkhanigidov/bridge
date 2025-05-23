#pragma once

#include "ExecutionError.h"
#include "../pch.h"

namespace engine::operation
{
    class PersistentObjectStore
    {
    public:
        static PersistentObjectStore& getInstance();

        PersistentObjectStore(const PersistentObjectStore&) = delete;
        PersistentObjectStore& operator=(const PersistentObjectStore&) = delete;

        void store(const std::string& id, rttr::variant objectVariant);

        rttr::variant& retrieveVariant(const std::string& id);
        void updateVariant(const std::string& id, const rttr::variant& updatedVariant);
        rttr::variant& accessVariant(const std::string& id);
        void debugPrintLiveObjects() const;
        template <typename T>
        std::shared_ptr<T> retrieveShared(const std::string& id);

        bool exists(const std::string& id) const;

        bool remove(const std::string& id);

        void clear();

    private:
        PersistentObjectStore() = default;
        ~PersistentObjectStore() = default;

        std::unordered_map<std::string, rttr::variant> m_store;
    };

    template <typename T>
    std::shared_ptr<T> PersistentObjectStore::retrieveShared(const std::string& id)
    {
        rttr::variant var = retrieveVariant(id);
        std::shared_ptr<T> ptr = var.get_value<std::shared_ptr<T>>();
        if (!ptr)
        {
            throw std::runtime_error("Object is not of the expected type: " + id);
        }

        return ptr;
    }
}
