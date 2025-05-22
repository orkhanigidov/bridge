#include "../../include/pch.h"
#include "../../include/operation/NodeFactory.h"
#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/FunctionNode.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/operation/ParameterConverter.h"

namespace engine::operation
{
    std::shared_ptr<BaseNode> NodeFactory::createNode(const nlohmann::json& nodeJson)
    {
        if (!nodeJson.contains("id") || !nodeJson.contains("type") || !nodeJson.contains("name"))
        {
            throw ExecutionError(ErrorType::InvalidFormat, "Missing required node fields (id, type, name)");
        }

        try
        {
            const std::string id = nodeJson["id"];
            const std::string type = nodeJson["type"];
            const std::string name = nodeJson["name"];

            std::shared_ptr<BaseNode> node;

            if (type == "object")
            {
                node = createObjectNode(id, name);
            }
            else if (type == "method")
            {
                node = createMethodNode(id, name);
            }
            else if (type == "function")
            {
                node = createFunctionNode(id, name);
            }
            else
            {
                throw ExecutionError(ErrorType::InvalidFormat, "Unknown node type: " + type);
            }

            setNodeParameters(node, nodeJson);
            setNodeDependencies(node, nodeJson);

            node->resolve();

            return node;
        }
        catch (const std::exception& e)
        {
            throw ExecutionError(ErrorType::FailedExecution, "Error creating node: " + std::string(e.what()));
        }
    }

    std::shared_ptr<BaseNode> NodeFactory::createObjectNode(const std::string& id, const std::string& name)
    {
        return std::make_shared<ObjectNode>(id, name);
    }

    std::shared_ptr<InvokableNode> NodeFactory::createMethodNode(const std::string& id, const std::string& name)
    {
        return std::make_shared<MethodNode>(id, name);
    }

    std::shared_ptr<InvokableNode> NodeFactory::createFunctionNode(const std::string& id, const std::string& name)
    {
        return std::make_shared<FunctionNode>(id, name);
    }

    void NodeFactory::setNodeParameters(const std::shared_ptr<BaseNode>& node, const nlohmann::json& nodeJson)
    {
        if (!nodeJson["parameters"].is_object())
        {
            throw ExecutionError(ErrorType::InvalidFormat, "Parameters field must be an object");
        }

        for (const auto& [key, value] : nodeJson["parameters"].items())
        {
            const Parameter parameter = parameterFromJson(value);
            node->setParameter(key, parameter);

            if (parameter.isReference()) node->addDependency(parameter.getReference());
        }
    }

    void NodeFactory::setNodeDependencies(const std::shared_ptr<BaseNode>& node, const nlohmann::json& nodeJson)
    {
        if (!nodeJson["dependencies"].is_array())
        {
            throw ExecutionError(ErrorType::InvalidFormat, "Dependencies field must be an array");
        }

        std::unordered_set existingDependencies(node->getDependencies().begin(), node->getDependencies().end());

        for (const auto& dependency : nodeJson["dependencies"])
        {
            if (existingDependencies.insert(dependency).second) node->addDependency(dependency);
        }
    }
} // namespace engine::operation
