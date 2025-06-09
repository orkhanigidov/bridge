#pragma once

#include "pch.hpp"

namespace engine::model
{
    class Class final
    {
      public:
        explicit Class(std::string_view alias, const rttr::type& type) : alias_{alias}, type_{type}
        {
        }

        std::string_view alias() const noexcept
        {
            return alias_;
        }

        const rttr::type& type() const noexcept
        {
            return type_;
        }

      private:
        std::string alias_;
        rttr::type type_;
    };
} // namespace engine::model