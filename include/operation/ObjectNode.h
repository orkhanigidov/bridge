#pragma once

#include "../pch.h"
#include "BaseNode.h"

namespace engine::operation
{
    class ObjectNode final : public BaseNode
    {
    public:
        ObjectNode(const std::string& id, const std::string& name);
        ~ObjectNode() override = default;

        bool hasInstance() const;

        void resolve() override;

    private:
        rttr::variant m_object;
    };
} // namespace engine::operation
