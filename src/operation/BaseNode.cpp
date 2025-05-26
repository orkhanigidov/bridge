#include "../../include/operation/BaseNode.h"

#include "../../include/pch.h"

namespace engine::operation
{
    BaseNode::BaseNode(std::string name, const NodeType type) : m_name(std::move(name)), m_type(type) {}

    std::string BaseNode::getName() const
    {
        return m_name;
    }

    NodeType BaseNode::getType() const
    {
        return m_type;
    }

    // const std::unordered_map<std::string, Parameter>& BaseNode::getParameters() const
    // {
    //     return m_parameters;
    // }
    //
    // void BaseNode::setParameter(const std::string& key, const Parameter& parameter)
    // {
    //     m_parameters[key] = parameter;
    // }

    void BaseNode::addDependency(const std::string& dependency)
    {
        m_dependencies.emplace_back(dependency);
    }

    const std::vector<std::string>& BaseNode::getDependencies() const
    {
        return m_dependencies;
    }
} // namespace engine::operation