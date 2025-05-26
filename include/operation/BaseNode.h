#pragma once

#include "../pch.h"

namespace engine::operation
{
    enum class NodeType
    {
        Object,
        Method,
        Function
    };

    class BaseNode
    {
      public:
        explicit BaseNode(std::string name, NodeType type);
        virtual ~BaseNode() = default;

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] NodeType getType() const;
        [[nodiscard]] const std::vector<std::string>& getDependencies() const;

        void addDependency(const std::string& dependency);

        virtual bool isValid() const = 0;

      protected:
        std::string m_name;
        NodeType m_type;
        std::vector<std::string> m_dependencies;
    };
} // namespace engine::operation