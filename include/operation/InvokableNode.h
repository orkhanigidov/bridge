#pragma once

#include "BaseNode.h"
#include "Result.h"

namespace engine::operation
{
    class InvokableNode : public BaseNode
    {
    public:
        using BaseNode::BaseNode;

        virtual Result invoke() = 0;
    };
} // namespace engine::operation
