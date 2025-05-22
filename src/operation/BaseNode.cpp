#include "../../include/pch.h"
#include "../../include/operation/BaseNode.h"

namespace engine::operation
{
    BaseNode::BaseNode(std::string id, const NodeType type, std::string name) :
        m_id(std::move(id)), m_type(type), m_name(std::move(name))
    {
    }

    const std::string& BaseNode::getId() const { return m_id; }

    NodeType BaseNode::getType() const { return m_type; }

    const std::string& BaseNode::getName() const { return m_name; }

    const std::unordered_map<std::string, Parameter>& BaseNode::getParameters() const { return m_parameters; }

    const std::vector<std::string>& BaseNode::getDependencies() const { return m_dependencies; }

    void BaseNode::setParameter(const std::string& key, const Parameter& parameter) { m_parameters[key] = parameter; }

    void BaseNode::addDependency(const std::string& dependency) { m_dependencies.push_back(dependency); }

    bool BaseNode::isValid() const { return m_resolved; }

    bool BaseNode::isResolved() const { return m_resolved; }

    std::vector<rttr::argument> BaseNode::prepareArguments(const rttr::method method) const
    {
        // method.get_parameter_infos();

        return {};
    }
} // namespace engine::operation
