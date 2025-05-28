#pragma once

#include "../pch.h"

#include "InvokableNode.h"
#include "Result.h"

namespace engine::operation
{
    class MethodNode final : public InvokableNode
    {
      public:
        explicit MethodNode(std::string_view name);

        void   resolve() override;
        Result invoke() override;

      private:
        rttr::instance instance_;
    };
} // namespace engine::operation