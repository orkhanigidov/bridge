#pragma once

#include "../pch.h"
#include "BaseNode.h"
#include "InvokableNode.h"

namespace engine::operation
{
    class NodeFactory final
    {
      public:
        NodeFactory(const NodeFactory&)            = delete;
        NodeFactory& operator=(const NodeFactory&) = delete;
        NodeFactory(NodeFactory&&)                 = delete;
        NodeFactory& operator=(NodeFactory&&)      = delete;

        static std::shared_ptr<BaseNode> createNode(const nlohmann::json& json);

      private:
        NodeFactory() = default;

        static std::shared_ptr<BaseNode> createObjectNode(std::string name);
        static std::shared_ptr<InvokableNode> createMethodNode(std::string name);
        static std::shared_ptr<InvokableNode> createFunctionNode(std::string name);

        static void setNodeParameters(const std::shared_ptr<BaseNode>& node, const nlohmann::json& json);
        static void addDependencies(const std::shared_ptr<BaseNode>& node, const nlohmann::json& json);
    };
} // namespace engine::operation