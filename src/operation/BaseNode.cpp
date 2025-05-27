#include "../../include/pch.h"

#include "../../include/operation/BaseNode.h"

namespace engine::operation
{
    BaseNode::BaseNode(std::string_view name, NodeType type) : name_(name), type_(type)
    {
        // dependencies_.reserve(4);
    }

    // void BaseNode::add_dependency(std::string_view dependency)
    // {
    //     dependencies_.emplace_back(dependency);
    // }
} // namespace engine::operation