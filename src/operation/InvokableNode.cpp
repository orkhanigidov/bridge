#include "../../include/pch.h"

#include "../../include/operation/InvokableNode.h"
#include "../../include/reflection/ReflectionRegistry.h"

namespace engine::operation
{
    // TODO: Initialize the method_ member variable in the constructor
    InvokableNode::InvokableNode(std::string_view name, NodeType type) : BaseNode(name, type)
    {
        parameters_.reserve(8);
    }

    void InvokableNode::set_parameter(std::string_view key, Parameter parameter)
    {
        parameters_.try_emplace(std::string{key}, std::move(parameter));
    }

    void InvokableNode::resolve()
    {
        const auto& registry = reflection::ReflectionRegistry::instance();
        const auto* method   = registry.get_method(name_);

        method_ = method->method();
    }

    bool InvokableNode::is_valid() const noexcept
    {
        return method_.is_valid();
    }
} // namespace engine::operation