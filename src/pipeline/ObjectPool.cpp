#include "pipeline/ObjectPool.hpp"

#include "pch.hpp"

namespace engine::pipeline
{
    ObjectPool& ObjectPool::instance()
    {
        static ObjectPool instance;
        return instance;
    }

    void ObjectPool::store(std::string_view id, rttr::variant object)
    {
        if (has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} +
                                     " already exists in the pool");

        objects_.emplace(id, std::move(object));
    }

    bool ObjectPool::has_object(std::string_view id) const noexcept
    {
        return objects_.find(std::string{id}) != objects_.end();
    }

    const rttr::variant& ObjectPool::get_object(std::string_view id) const
    {
        if (!has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} +
                                     " does not exist in the pool");

        return objects_.at(std::string{id});
    }

    bool ObjectPool::remove(std::string_view id)
    {
        if (!has_object(id))
            throw std::runtime_error("Object with ID " + std::string{id} +
                                     " does not exist in the pool");

        return objects_.erase(std::string{id}) > 0;
    }

    void ObjectPool::clear_all()
    {
        objects_.clear();
    }

    rttr::variant ObjectPool::resolve_reference(std::string_view reference) const
    {
        return get_object(reference);
    }
} // namespace engine::pipeline