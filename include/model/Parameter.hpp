#pragma once

#include "pch.hpp"

namespace engine::model
{
    class Parameter final
    {
      public:
        explicit Parameter(std::string_view name, const rttr::type& type,
                           rttr::variant default_value = rttr::variant(), bool is_reference = false)
            : name_{name}, type_{type}, default_value_{std::move(default_value)},
              is_reference_{is_reference}
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const rttr::type& type() const noexcept
        {
            return type_;
        }

        const rttr::variant& default_value() const noexcept
        {
            return default_value_;
        }

        bool is_reference() const noexcept
        {
            return is_reference_;
        }

      private:
        std::string name_;
        rttr::type type_;
        rttr::variant default_value_;
        bool is_reference_;
    };
} // namespace engine::model