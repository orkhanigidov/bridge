#include "../../include/pch.h"

#include "../../include/operation/GraphExecutor.h"
#include "../../include/operation/NodeFactory.h"

namespace engine::operation
{
    GraphExecutor& GraphExecutor::instance()
    {
        static GraphExecutor instance;
        return instance;
    }

    void GraphExecutor::load_json(const nlohmann::json& json)
    {
        if (!json.contains("graph") || !json["graph"].contains("nodes"))
            throw std::invalid_argument("Invalid graph JSON format");

        clear_all();

        for (const auto& node : json["graph"]["nodes"])
        {
            original_order_.emplace_back(NodeFactory::create_node(node));
        }

        // sort_nodes_topologically();
    }

    void GraphExecutor::execute() const
    {
        for (const auto& node : original_order_)
        {
            node->resolve();

            if (auto* invokable_node = dynamic_cast<InvokableNode*>(node.get()))
                invokable_node->invoke();
        }
    }

    void GraphExecutor::clear_all()
    {
        original_order_.clear();
        execution_order_.clear();
    }

    // bool GraphExecutor::has_cycle() const {}

    // void GraphExecutor::sort_nodes_topologically() {}
} // namespace engine::operation
