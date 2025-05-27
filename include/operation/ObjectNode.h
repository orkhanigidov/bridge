#pragma once

#include "../pch.h"

#include "BaseNode.h"

namespace engine::operation
{
    class ObjectNode final : public BaseNode
    {
      public:
        explicit ObjectNode(std::string_view name);

        [[nodiscard]] std::string_view id() const noexcept
        {
            return id_;
        }

        [[nodiscard]] const rttr::variant& object() const noexcept
        {
            return object_;
        }

        [[nodiscard]] bool has_object() const noexcept;

        void               resolve() override;
        [[nodiscard]] bool is_valid() const noexcept override;

      private:
        std::string   id_;
        rttr::variant object_;
    };
} // namespace engine::operation