#include "../../include/operation/ObjectStore.h"

#include "../../include/pch.h"

namespace engine::operation
{
    ObjectStore& ObjectStore::getInstance()
    {
        static ObjectStore instance;
        return instance;
    }

    void ObjectStore::storeObject(const std::string& id, const rttr::type& type)
    {
        if (hasObject(id))
            throw std::runtime_error("Object with ID " + id + " already exists in the store");

        m_references.emplace(id, type);
    }

    bool ObjectStore::hasObject(const std::string& id)
    {
        return m_references.find(id) != m_references.end();
    }

    const rttr::variant& ObjectStore::getObject(const std::string& id)
    {
        if (hasObject(id))
            throw std::runtime_error("Object with ID " + id + " does not exist in the store");

        return m_references.at(id);
    }

    void ObjectStore::removeObject(const std::string& id)
    {
        if (hasObject(id))
            throw std::runtime_error("Object with ID " + id + " does not exist in the store");

        m_references.erase(id);
    }

    void ObjectStore::updateObject(const std::string& id, const rttr::variant& variant)
    {
        if (!hasObject(id))
            throw std::runtime_error("Object with ID " + id + " does not exist in the store");

        m_references[id] = variant;
    }

    void ObjectStore::clearAll()
    {
        m_references.clear();
    }
} // namespace engine::operation