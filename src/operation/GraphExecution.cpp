#include "../../include/pch.h"
#include "../../include/operation/GraphExecution.h"
#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/NodeFactory.h"

namespace engine::operation
{
    void GraphExecution::loadFromJson(const nlohmann::json& json)
    {
        if (!json.contains("graph") || !json["graph"].contains("nodes"))
        {
            throw ExecutionError(ErrorType::InvalidFormat, "Invalid graph JSON format");
        }

        clear();

        try
        {
            for (const auto& nodeJson : json["graph"]["nodes"])
            {
                auto node = NodeFactory::createNode(nodeJson);
                addNode(node);
                m_originalOrder.push_back(node->getId());
            }

            sortNodesTopologically();
        }
        catch (const std::exception& e)
        {
            throw ExecutionError(ErrorType::InvalidFormat, "JSON parsing error: " + std::string(e.what()));
        }
    }

    void GraphExecution::executeGraph()
    {
        if (getNodes().empty())
        {
            return;
        }

        m_results.clear();

        for (const auto& nodeId : m_originalOrder)
        {
            const auto node = getNode(nodeId);
            const auto invokable = std::dynamic_pointer_cast<InvokableNode>(node);
            if (invokable)
            {
                const Result result = invokable->invoke();
                if (result.hasValue())
                {
                    m_results[nodeId] = result;
                }
            }
        }
    }

    std::shared_ptr<BaseNode> GraphExecution::getNode(const std::string& id) const { return m_nodes.at(id); }

    const std::unordered_map<std::string, std::shared_ptr<BaseNode>>& GraphExecution::getNodes() const { return m_nodes; }

    void GraphExecution::addNode(const std::shared_ptr<BaseNode>& node) { m_nodes[node->getId()] = node; }

    const Result& GraphExecution::getResult(const std::string& nodeId) const { return m_results.at(nodeId); }

    void GraphExecution::clear()
    {
        m_nodes.clear();
        m_originalOrder.clear();
        m_executionOrder.clear();
    }

    bool GraphExecution::hasCycle() const
    {
        std::unordered_map<std::string, bool> visiting;
        std::unordered_set<std::string> visited;

        std::function<bool(const std::string&)> detectCycle = [&](const std::string& nodeId)
        {
            if (visiting[nodeId])
            {
                return true;
            }
            if (visited.find(nodeId) != visited.end())
            {
                return false;
            }

            visiting[nodeId] = true;
            const auto& node = m_nodes.at(nodeId);
            for (const auto& dependency : node->getDependencies())
            {
                if (m_nodes.find(dependency) != m_nodes.end())
                {
                    if (detectCycle(dependency))
                    {
                        return true;
                    }
                }
            }

            visiting[nodeId] = false;
            visited.insert(nodeId);
            return false;
        };

        for (const auto& [id, _] : m_nodes)
        {
            if (visited.find(id) == visited.end())
            {
                if (detectCycle(id))
                {
                    return true;
                }
            }
        }

        return false;
    }

    void GraphExecution::sortNodesTopologically()
    {
        if (hasCycle())
        {
            throw ExecutionError(ErrorType::FailedValidation, "Cannot sort nodes: graph contains cycles");
        }

        m_executionOrder.clear();

        std::unordered_set<std::string> visited;
        std::function<void(const std::string&)> visit = [&](const std::string& nodeId)
        {
            if (visited.find(nodeId) != visited.end())
            {
                return;
            }

            visited.insert(nodeId);

            const auto node = m_nodes.at(nodeId);
            for (const auto& dependency : node->getDependencies())
            {
                if (m_nodes.find(dependency) != m_nodes.end())
                {
                    visit(dependency);
                }
            }

            m_executionOrder.push_back(nodeId);
        };

        for (const auto& [id, _] : m_nodes)
        {
            if (visited.find(id) == visited.end())
            {
                visit(id);
            }
        }
    }
} // namespace engine::operation
