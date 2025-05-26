#include "../../include/operation/ObjectNode.h"

#include "../../include/operation/BaseNode.h"
#include "../../include/pch.h"
#include "../../include/reflection/ReflectionRegistry.h"

namespace engine::operation
{
    ObjectNode::ObjectNode(std::string name) : BaseNode(std::move(name), NodeType::Object)
    {
        const model::Class* type = reflection::ReflectionRegistry::getInstance().getClass(name);

        m_id     = type->getId();
        m_object = type->getType().create();
    }

    rttr::variant ObjectNode::getObject() const
    {
        return m_object;
    }

    bool ObjectNode::hasInstance() const
    {
        return m_object.is_valid();
    }

    bool ObjectNode::isValid() const
    {
        return m_object.is_valid();
    }
} // namespace engine::operation