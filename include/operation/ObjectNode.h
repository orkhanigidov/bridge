#pragma once

#include "../pch.h"
#include "BaseNode.h"

namespace engine::operation
{
    class ObjectNode final : public BaseNode
    {
      public:
        explicit ObjectNode(std::string name);
        ~ObjectNode() override = default;

        [[nodiscard]] rttr::variant getObject() const;
        [[nodiscard]] bool hasInstance() const;

        void resolve();

        bool isValid() const override;

      private:
        std::string m_id;
        rttr::variant m_object;
    };
} // namespace engine::operation