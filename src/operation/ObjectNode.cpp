#include "../../include/pch.h"

#include "../../include/operation/ObjectNode.h"
#include "../../include/operation/BaseNode.h"
#include "../../include/operation/ObjectPool.h"
#include "../../include/reflection/ReflectionRegistry.h"

namespace engine::operation
{
    ObjectNode::ObjectNode(std::string_view name) : BaseNode(name, NodeType::Object) {}

    bool ObjectNode::has_object() const noexcept
    {
        return object_.is_valid();
    }

    void ObjectNode::resolve()
    {
        const auto& registry   = reflection::ReflectionRegistry::instance();
        const auto* class_type = registry.get_class(name());

        id_     = class_type->id();
        object_ = class_type->type().create();

        if (!ObjectPool::instance().has_object(id_))
            ObjectPool::instance().store(id_, std::make_unique<ObjectNode>(*this));
    }

    bool ObjectNode::is_valid() const noexcept
    {
        return object_.is_valid();
    }
} // namespace engine::operation