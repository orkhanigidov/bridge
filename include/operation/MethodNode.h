#pragma once

#include "../pch.h"
#include "InvokableNode.h"
#include "Result.h"

namespace engine::operation
{
    class MethodNode final : public InvokableNode
    {
      public:
        explicit MethodNode(std::string name);
        ~MethodNode() override = default;

        // rttr::type getObject() const;

        Result invoke() override;

      private:
        // std::optional<rttr::type> m_object;
    };
} // namespace engine::operation