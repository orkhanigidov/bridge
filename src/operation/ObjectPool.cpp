#include "../../include/pch.h"

#include "../../include/operation/ObjectPool.h"

namespace engine::operation
{
    ObjectPool& ObjectPool::instance()
    {
        static ObjectPool instance;
        return instance;
    }

    void ObjectPool::store(std::string_view id, std::unique_ptr<ObjectNode> object)
    {
        if (has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} + " already exists in the pool");

        // TODO: Check emplace vs try_emplace for performance
        objects_.emplace(id, std::move(object));
    }

    bool ObjectPool::has_object(std::string_view id)
    {
        return objects_.find(std::string{id}) != objects_.end();
    }

    const rttr::variant& ObjectPool::get_object(std::string_view id)
    {
        if (has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} + " does not exist in the pool");

        return objects_.at(std::string{id});
    }

    bool ObjectPool::remove(std::string_view id)
    {
        if (has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} + " does not exist in the pool");

        return objects_.erase(std::string{id});
    }

    void ObjectPool::clear_all()
    {
        objects_.clear();
    }

    // void ObjectPool::update(std::string_view id, const rttr::variant& variant)
    // {
    //     if (has_object(id))
    //         throw std::runtime_error("Object with ID " + std::string{id} + " does not exist in the pool");
    //
    //     objects_[std::string{id}] = variant;
    // }
} // namespace engine::operation