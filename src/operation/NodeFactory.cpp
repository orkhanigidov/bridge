#include "../../include/operation/NodeFactory.h"

#include "../../include/model/Parameter.h"
#include "../../include/operation/FunctionNode.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/pch.h"
#include "../../include/serialization/MethodSerializer.h"

namespace engine::operation
{
    std::shared_ptr<BaseNode> NodeFactory::createNode(const nlohmann::json& json)
    {
        if (!json.contains("type") || !json.contains("name"))
            throw std::invalid_argument("Missing required node fields (type, name)");

        const std::string type = json["type"];
        const std::string name = json["name"];

        std::shared_ptr<BaseNode> node;

        if (type == "object")
            node = createObjectNode(name);
        else if (type == "method")
            node = createMethodNode(name);
        else if (type == "function")
            node = createFunctionNode(name);
        else
            throw std::invalid_argument("Unknown node type" + type);

        setNodeParameters(node, json);
        addDependencies(node, json);

        return node;
    }

    std::shared_ptr<BaseNode> NodeFactory::createObjectNode(std::string name)
    {
        return std::make_shared<ObjectNode>(std::move(name));
    }

    std::shared_ptr<InvokableNode> NodeFactory::createMethodNode(std::string name)
    {
        return std::make_shared<MethodNode>(std::move(name));
    }

    std::shared_ptr<InvokableNode> NodeFactory::createFunctionNode(std::string name)
    {
        return std::make_shared<FunctionNode>(std::move(name));
    }

    void NodeFactory::setNodeParameters(const std::shared_ptr<BaseNode>& node, const nlohmann::json& json)
    {
        if (!json["parameters"].is_object())
            throw std::invalid_argument("Parameters field must be an object");

        for (const auto& [key, value] : json["parameters"].items())
        {
            const model::Parameter parameter = serialization::MethodSerializer::parameterFromJson(value);
            node->setParameter(key, parameter);

            if (parameter.isReference())
                node->addDependency(parameter.getReference());
        }
    }

    void NodeFactory::addDependencies(const std::shared_ptr<BaseNode>& node, const nlohmann::json& json)
    {
        if (!json["dependencies"] || !json["dependencies"].is_array())
            throw std::invalid_argument("Dependencies field must be an array");

        std::unordered_set dependencies(node->getDependencies().begin(), node->getDependencies().end());

        for (const auto& dependency : json["dependencies"])
        {
            if (dependencies.insert(dependency).second)
                node->addDependency(dependency);
        }
    }
} // namespace engine::operation