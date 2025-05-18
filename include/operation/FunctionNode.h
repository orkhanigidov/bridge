#pragma once

#include "../pch.h"
#include "InvokableNode.h"
#include "Result.h"

namespace engine::operation
{
    class FunctionNode final : public InvokableNode
    {
    public:
        FunctionNode(const std::string& id, const std::string& name);
        ~FunctionNode() override = default;

        rttr::method getMethod() const;

        Result invoke() override;
        void resolve() override;

    private:
        std::optional<rttr::method> m_method;
    };
} // namespace engine::operation
