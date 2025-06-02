#include "pipeline/NewStep.hpp"

#include "pch.hpp"
#include "pipeline/ObjectPool.hpp"
#include "pipeline/Step.hpp"
#include "reflection/ReflectionRegistry.hpp"

namespace engine::pipeline
{
    NewStep::NewStep(const nlohmann::json& json) : Step(json)
    {
        if (!alias_.has_value())
            throw std::invalid_argument("New step must have an 'as' field for object alias");

        class_name_ = name_;
    }

    void NewStep::execute()
    {
        const auto& registry   = reflection::ReflectionRegistry::instance();
        const auto* class_info = registry.get_class(class_name_);

        if (!class_info)
            throw std::runtime_error("Class not found: " + class_name_);

        auto object = class_info->type().create();
        if (!object.is_valid())
            throw std::runtime_error("Failed to create instance of: " + class_name_);

        pipeline::ObjectPool::instance().store(alias_.value(), std::move(object));
    }
} // namespace engine::pipeline