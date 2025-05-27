#pragma once

#include "../pch.h"

namespace engine::operation
{
    enum class NodeType : std::uint8_t
    {
        Object,
        Method,
        Function
    };

    class BaseNode
    {
      public:
        explicit BaseNode(std::string_view name, NodeType type);

        virtual ~BaseNode() = default;

        [[nodiscard]] std::string_view name() const noexcept
        {
            return name_;
        }

        [[nodiscard]] NodeType type() const noexcept
        {
            return type_;
        }

        // [[nodiscard]] const std::vector<std::string>& dependencies() const noexcept
        // {
        //     return dependencies_;
        // }

        // void add_dependency(std::string_view dependency);

        virtual void               resolve()                 = 0;
        [[nodiscard]] virtual bool is_valid() const noexcept = 0;

      protected:
        std::string name_;
        NodeType    type_;
        // std::vector<std::string> dependencies_;
    };
} // namespace engine::operation