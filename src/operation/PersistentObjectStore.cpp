#include "../../include/operation/PersistentObjectStore.h"

#include "../../include/pch.h"
#include "../../include/operation/ExecutionError.h"

namespace engine::operation
{
    PersistentObjectStore& PersistentObjectStore::getInstance()
    {
        static PersistentObjectStore instance;
        return instance;
    }

    void PersistentObjectStore::store(const std::string& id, rttr::variant objectVariant)
    {
        if (id.empty())
            throw std::invalid_argument("Object ID cannot be empty");

        if (!objectVariant.is_valid())
            throw std::invalid_argument("Object is not valid");

        m_store[id] = objectVariant;
    }

    rttr::variant& PersistentObjectStore::accessVariant(const std::string& id)
    {
        auto it = m_store.find(id);
        if (it == m_store.end())
            throw std::invalid_argument("Object does not exist");

        return it->second;
    }


    rttr::variant& PersistentObjectStore::retrieveVariant(const std::string& id)
    {
        auto it = m_store.find(id);
        if (it != m_store.end() && it->second.is_valid())
        {
            return it->second;
        }

        static rttr::variant invalid;
        return invalid;
    }

    void PersistentObjectStore::updateVariant(const std::string& id, const rttr::variant& updatedVariant)
    {
        auto it = m_store.find(id);
        if (it != m_store.end())
        {
            it->second = updatedVariant;
        }
    }


    void PersistentObjectStore::debugPrintLiveObjects() const
    {
        std::cout << "Live objects in store:\n";
        for (const auto& pair : m_store)
        {
            const auto& id = pair.first;
            const auto& var = pair.second;
            if (var.is_valid())
            {
                auto typeName = var.get_type().get_name().to_string();
                std::cout << "  - ID: " << id
                    << ", Type: " << typeName
                    << ", Address: " << var.get_value<void*>() // raw pointer if applicable
                    << "\n";
            }
        }
    }

    bool PersistentObjectStore::exists(const std::string& id) const
    {
        if (id.empty())
            return false;

        return m_store.count(id) > 0;
    }

    bool PersistentObjectStore::remove(const std::string& id)
    {
        if (id.empty())
            return false;

        return m_store.erase(id) > 0;
    }

    void PersistentObjectStore::clear()
    {
        m_store.clear();
    }
}
