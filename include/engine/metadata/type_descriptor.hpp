#pragma once

#include "pch.hpp"
#include "type.hpp"

namespace engine::metadata
{
    class type_descriptor final
    {
      public:
        explicit type_descriptor(data_type type) : type_(std::move(type)) {}

        explicit type_descriptor(std::string_view custom_type_name)
            : type_(data_type::Custom), custom_type_name_(custom_type_name)
        {
        }

        data_type type() const noexcept
        {
            return type_;
        }

        std::string_view custom_type_name() const noexcept
        {
            return custom_type_name_;
        }

        bool is_custom_type() const noexcept
        {
            return type_ == data_type::Custom;
        }

      private:
        data_type type_;
        std::string custom_type_name_;
    };
} // namespace engine::metadata