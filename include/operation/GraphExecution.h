#pragma once

#include "../pch.h"
#include "BaseNode.h"
#include "Result.h"

namespace engine::operation
{
    class GraphExecution
    {
    public:
        GraphExecution() = default;
        ~GraphExecution() = default;

        std::shared_ptr<BaseNode> getNode(const std::string& id) const;
        const std::unordered_map<std::string, std::shared_ptr<BaseNode>>& getNodes() const;
        void addNode(const std::shared_ptr<BaseNode>& node);
        const Result& getResult(const std::string& nodeId) const;

        void loadFromJson(const nlohmann::json& json);
        void executeGraph();
        void clear();

    private:
        std::unordered_map<std::string, std::shared_ptr<BaseNode>> m_nodes;
        std::unordered_map<std::string, Result> m_results;
        std::vector<std::string> m_originalOrder;
        std::vector<std::string> m_executionOrder;

        bool hasCycle() const;
        void sortNodesTopologically();
    };
} // namespace engine::operation
