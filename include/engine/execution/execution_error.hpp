#pragma once

#include "pch.hpp"

namespace engine::execution
{
    enum class error_type : std::uint8_t
    {
        invalid_argument,
        invalid_format,
        invalid_conversion,
        failed_execution,
        failed_conversion,
        failed_validation
    };

    class execution_error final : public std::runtime_error
    {
      public:
        explicit execution_error(error_type type, std::string_view message)
            : std::runtime_error(std::string(message)), error_type_(type)
        {
        }

        error_type type() const noexcept
        {
            return error_type_;
        }

      private:
        error_type error_type_;
    };
} // namespace engine::execution