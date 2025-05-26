#include "../../include/operation/InvokableNode.h"

#include "../../include/pch.h"

namespace engine::operation
{
    InvokableNode::InvokableNode(std::string name, const NodeType type) : BaseNode(std::move(name), type) {}

    rttr::method InvokableNode::getMethod() const
    {
        return m_method;
    }

    bool InvokableNode::isValid() const
    {
        return m_method.is_valid();
    }
} // namespace engine::operation