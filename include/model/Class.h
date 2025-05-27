#pragma once

#include "../pch.h"

namespace engine::model
{
    class Class final
    {
      public:
        explicit Class(std::string_view id, const rttr::type& type);

        [[nodiscard]] std::string_view id() const noexcept
        {
            return id_;
        }

        [[nodiscard]] const rttr::type& type() const noexcept
        {
            return type_;
        }

      private:
        std::string id_;
        rttr::type  type_;
    };
} // namespace engine::model