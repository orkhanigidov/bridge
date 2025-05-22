#pragma once

#include "../pch.h"
#include "BaseNode.h"
#include "InvokableNode.h"

namespace engine::operation
{
    class NodeFactory
    {
    public:
        NodeFactory() = delete;
        ~NodeFactory() = delete;

        static std::shared_ptr<BaseNode> createNode(const nlohmann::json& nodeJson);

    private:
        static std::shared_ptr<BaseNode> createObjectNode(const std::string& id, const std::string& name);
        static std::shared_ptr<InvokableNode> createMethodNode(const std::string& id, const std::string& name);
        static std::shared_ptr<InvokableNode> createFunctionNode(const std::string& id, const std::string& name);

        static void setNodeParameters(const std::shared_ptr<BaseNode>& node, const nlohmann::json& nodeJson);
        static void setNodeDependencies(const std::shared_ptr<BaseNode>& node, const nlohmann::json& nodeJson);
    };
} // namespace engine::operation
