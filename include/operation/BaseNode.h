#pragma once

#include "../pch.h"
#include "Parameter.h"

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
        BaseNode(std::string id, NodeType type, std::string name);
        virtual ~BaseNode() = default;

        const std::string& getId() const;
        NodeType getType() const;
        const std::string& getName() const;
        const std::unordered_map<std::string, Parameter>& getParameters() const;
        const std::vector<std::string>& getDependencies() const;

        void setParameter(const std::string& key, const Parameter& parameter);
        void addDependency(const std::string& dependency);

        bool isValid() const;
        bool isResolved() const;

        virtual void resolve() = 0;

    protected:
        std::vector<rttr::variant> prepareArguments(rttr::method method) const;

        std::string m_id;
        NodeType m_type;
        std::string m_name;
        std::unordered_map<std::string, Parameter> m_parameters;
        std::vector<std::string> m_dependencies;
        bool m_resolved{false};
    };
} // namespace engine::operation
