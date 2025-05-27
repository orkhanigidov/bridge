#pragma once

#include "../pch.h"

#include "InvokableNode.h"
#include "Result.h"

namespace engine::operation
{
    class FunctionNode final : public InvokableNode
    {
      public:
        explicit FunctionNode(std::string_view name);

        Result invoke() override;
    };
} // namespace engine::operation