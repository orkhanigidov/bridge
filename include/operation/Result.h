#pragma once

#include "../pch.h"

namespace engine::operation
{
    class Result final
    {
      public:
        explicit Result(rttr::variant value);

        [[nodiscard]] bool                 has_value() const noexcept;
        [[nodiscard]] const rttr::variant& value() const noexcept;

      private:
        rttr::variant value_;
    };
} // namespace engine::operation