#include "../../include/pch.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/operation/BaseNode.h"

namespace engine::operation
{
    ObjectNode::ObjectNode(const std::string& id, const std::string& name) : BaseNode(id, NodeType::Object, name)
    {
    }

    bool ObjectNode::hasInstance() const
    {
        return m_instance.value().is_valid();
    }

    void ObjectNode::resolve()
    {
        if (!isResolved())
            m_instance.emplace(rttr::type::get_by_name(m_name).create());

        m_resolved = true;
    }
} // namespace engine::operation
