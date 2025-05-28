#include "../../include/pch.h"

#include "../../include/operation/NodeFactory.h"
#include "../../include/model/Parameter.h"
#include "../../include/operation/FunctionNode.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/serialization/MethodSerializer.h"

namespace engine::operation
{
    std::unique_ptr<BaseNode> NodeFactory::create_node(const nlohmann::json& json)
    {
        if (!json.contains("type") || !json.contains("name"))
            throw std::invalid_argument("Missing required node fields (type, name)");

        const auto type = json["type"].get<std::string_view>();
        const auto name = json["name"].get<std::string_view>();

        std::unique_ptr<BaseNode> node;

        if (type == "object")
            node = create_object_node(name);
        else if (type == "method")
            node = create_method_node(name);
        else if (type == "function")
            node = create_function_node(name);
        else
            throw std::invalid_argument("Unknown node type" + std::string{type});

        if (node->type() == NodeType::Method || node->type() == NodeType::Function)
            set_parameters(dynamic_cast<InvokableNode&>(*node), json);

        // set_dependencies(*node, json);

        return node;
    }

    std::unique_ptr<BaseNode> NodeFactory::create_object_node(std::string_view name)
    {
        return std::make_unique<ObjectNode>(name);
    }

    std::unique_ptr<InvokableNode> NodeFactory::create_method_node(std::string_view name)
    {
        return std::make_unique<MethodNode>(name);
    }

    std::unique_ptr<InvokableNode> NodeFactory::create_function_node(std::string_view name)
    {
        return std::make_unique<FunctionNode>(name);
    }

    void NodeFactory::set_parameters(InvokableNode& node, const nlohmann::json& json)
    {
        const auto& parameters = json["parameters"];

        if (!parameters || !parameters.is_object())
            throw std::invalid_argument("Parameters field must be a JSON object");

        for (const auto& [key, value] : parameters.items())
        {
            const auto parameter = serialization::MethodSerializer::parameter_from_json(value);

            if (!parameter.has_value())
                throw std::invalid_argument("Invalid parameter definition for key: " + key);

            node.set_parameter(key, parameter.value());

            // if (parameter->is_reference())
            //     node.add_dependency(parameter->name());
        }
    }

    // void NodeFactory::set_dependencies(BaseNode& node, const nlohmann::json& json)
    // {
    //     const auto& dependencies = json["dependencies"];
    //
    //     if (!dependencies || !dependencies.is_array())
    //         throw std::invalid_argument("Dependencies field must be a JSON array");
    //
    //     for (const auto& dependency : dependencies)
    //     {
    //         node.add_dependency(dependency.get<std::string_view>());
    //     }
    // }
} // namespace engine::operation