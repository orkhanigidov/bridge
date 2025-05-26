#pragma once

#include "../pch.h"

namespace engine::operation
{
    class Result final
    {
      public:
        Result() = default;
        explicit Result(const rttr::variant& variant);

        [[nodiscard]] rttr::variant getVariant() const;
        [[nodiscard]] bool hasVariant() const;

      private:
        rttr::variant m_variant;
        bool m_hasVariant{false};
    };
} // namespace engine::operation