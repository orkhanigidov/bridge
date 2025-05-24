#include "../../include/pch.h"
#include "../../include/operation/BaseNode.h"
#include "../../include/operation/PersistentObjectStore.h"

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

    std::vector<rttr::variant> BaseNode::prepareArguments(const rttr::method method) const
    {
        std::vector<rttr::variant> args;
        const auto& parameterInfos = method.get_parameter_infos();

        for (const auto& parameterInfo : parameterInfos) {
            const std::string paramName = parameterInfo.get_name().to_string();

            auto it = m_parameters.find(paramName);
            if (it == m_parameters.end()) {
                continue; // Parameter not found
            }
            const Parameter& parameter = it->second;

            if (parameter.isReference()) {
                std::string refId = parameter.getReference();
                rttr::variant &node = PersistentObjectStore::getInstance().retrieveVariant(refId);
                args.push_back(node);
                PersistentObjectStore::getInstance().debugPrintLiveObjects();
            } else {
                if (parameter.isInt()) {
                    args.push_back(parameter.asInt());
                } else if (parameter.isFloat()) {
                    args.push_back(parameter.asFloat());
                } else if (parameter.isDouble()) {
                    args.push_back(parameter.asDouble());
                } else if (parameter.isBool()) {
                    args.push_back(parameter.asBool());
                } else if (parameter.isString()) {
                    args.push_back(parameter.asString());
                } else {
                    throw std::runtime_error("Unsupported parameter type");
                }
            }
        }

        return args;
    }
} // namespace engine::operation
