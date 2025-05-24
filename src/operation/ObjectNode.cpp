#include "../../include/pch.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/operation/BaseNode.h"
#include "../../include/operation/PersistentObjectStore.h"

namespace engine::operation
{
    ObjectNode::ObjectNode(const std::string& id, const std::string& name) : BaseNode(id, NodeType::Object, name)
    {
    }

    bool ObjectNode::hasInstance() const
    {
        return m_object.is_valid();
    }

    void ObjectNode::resolve()
    {
        if (!isResolved()) {
            m_object = rttr::type::get_by_name(m_name).create();
            PersistentObjectStore::getInstance().store(m_id, m_object);
        }

        m_resolved = true;
    }
} // namespace engine::operation
