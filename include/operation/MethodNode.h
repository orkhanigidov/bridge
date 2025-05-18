#pragma once

#include "../pch.h"
#include "InvokableNode.h"
#include "Result.h"

namespace engine::operation
{
    class MethodNode final : public InvokableNode
    {
    public:
        MethodNode(const std::string& id, const std::string& name);
        ~MethodNode() override = default;

        rttr::method getMethod() const;
        rttr::type getObject() const;

        Result invoke() override;
        void resolve() override;

    private:
        std::optional<rttr::method> m_method;
        std::optional<rttr::type> m_object;
    };
} // namespace engine::operation
