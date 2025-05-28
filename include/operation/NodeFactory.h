#pragma once

#include "../pch.h"

#include "BaseNode.h"
#include "InvokableNode.h"

namespace engine::operation
{
    class NodeFactory final
    {
      public:
        static std::unique_ptr<BaseNode> create_node(const nlohmann::json& json);

      private:
        static std::unique_ptr<BaseNode>      create_object_node(std::string_view name);
        static std::unique_ptr<InvokableNode> create_method_node(std::string_view name);
        static std::unique_ptr<InvokableNode> create_function_node(std::string_view name);

        static void set_parameters(InvokableNode& node, const nlohmann::json& json);
        // static void set_dependencies(BaseNode& node, const nlohmann::json& json);
    };
} // namespace engine::operation