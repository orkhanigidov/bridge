#pragma once

#include "../pch.h"

#include "BaseNode.h"

namespace engine::operation
{
    class GraphExecutor final
    {
      public:
        static GraphExecutor& instance();

        void load_json(const nlohmann::json& json);
        void execute() const;
        void clear_all();

      private:
        std::vector<std::unique_ptr<BaseNode>> original_order_;
        std::vector<std::unique_ptr<BaseNode>> execution_order_;

        GraphExecutor() = default;

        // [[nodiscard]] bool has_cycle() const;
        // void               sort_nodes_topologically();
    };
} // namespace engine::operation