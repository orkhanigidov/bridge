#pragma once

#include "TypeDesc.hpp"

namespace engine::model
{
    class ParamDesc
    {
      public:
        ParamDesc(std::string_view name, TypeDesc type, bool is_optional = false,
                  std::string_view default_value = "")
            : name_(name), type_(std::move(type)), is_optional_(is_optional),
              default_value_(default_value)
        {
        }

        std::string_view name() const noexcept
        {
            return name_;
        }

        const TypeDesc& type() const noexcept
        {
            return type_;
        }

        bool is_optional() const noexcept
        {
            return is_optional_;
        }

        std::string_view default_value() const noexcept
        {
            return default_value_;
        }

        bool has_default_value() const noexcept
        {
            return !default_value_.empty();
        }

      private:
        std::string name_;
        TypeDesc type_;
        bool is_optional_;
        std::string default_value_;
    };
} // namespace engine::model