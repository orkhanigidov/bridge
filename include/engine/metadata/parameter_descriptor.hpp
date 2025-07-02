#pragma once

#include "type_descriptor.hpp"

namespace engine::metadata
{
    class parameter_descriptor final
    {
      public:
        parameter_descriptor(std::string_view name, type_descriptor type,
                             std::string_view default_value)
            : name_(name), type_(std::move(type)), default_value_(default_value)
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const type_descriptor& type() const noexcept
        {
            return type_;
        }

        std::string_view default_value() const noexcept
        {
            return default_value_.value_or("");
        }

        bool has_default_value() const noexcept
        {
            return default_value_.has_value();
        }

      private:
        std::string name_;
        type_descriptor type_;
        std::optional<std::string> default_value_;
    };
} // namespace engine::metadata