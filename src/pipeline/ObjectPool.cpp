#include "pipeline/ObjectPool.hpp"

#include "pch.hpp"

namespace engine::pipeline
{
    void ObjectPool::store(std::string_view alias, rttr::variant object)
    {
        if (has_object(alias))
            throw std::runtime_error("Object with ID " + std::string{alias} +
                                     " already exists in the pool");

        objects_.emplace(alias, std::move(object));
    }

    bool ObjectPool::has_object(std::string_view alias) const noexcept
    {
        return objects_.find(std::string{alias}) != objects_.end();
    }

    const rttr::variant& ObjectPool::get_object(std::string_view alias) const
    {
        if (!has_object(alias))
            throw std::runtime_error("Object with ID " + std::string{alias} +
                                     " does not exist in the pool");

        return objects_.at(std::string{alias});
    }

    void ObjectPool::update_object(std::string_view alias, const rttr::variant& object)
    {
        if (!has_object(alias))
            throw std::runtime_error("Object not found for update: " + std::string{alias});

        objects_[std::string{alias}] = object;
        std::cout << "Updated object '" << alias << "' in pool" << std::endl;
    }

    void ObjectPool::clear_all()
    {
        objects_.clear();
    }
} // namespace engine::pipeline