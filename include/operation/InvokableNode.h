#pragma once

#include "BaseNode.h"
#include "Result.h"

namespace engine::operation
{
    class InvokableNode : public BaseNode
    {
      public:
        explicit InvokableNode(std::string name, NodeType type);
        ~InvokableNode() override = default;

        [[nodiscard]] rttr::method getMethod() const;

        [[nodiscard]] bool isValid() const override;

        virtual Result invoke() = 0;

      protected:
        rttr::method m_method;
    };
} // namespace engine::operation