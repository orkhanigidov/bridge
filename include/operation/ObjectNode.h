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
        std::optional<rttr::instance> m_instance;
    };
} // namespace engine::operation
