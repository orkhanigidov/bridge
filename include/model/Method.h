#pragma once

#include "../pch.h"

#include "Parameter.h"

namespace engine::model
{
    class Method final
    {
      public:
        explicit Method(const rttr::method& method, const rttr::type& return_type, std::vector<Parameter> parameters,
                        bool is_static, std::string_view category = "", std::string_view description = "");

        [[nodiscard]] const rttr::method& method() const noexcept
        {
            return method_;
        }

        [[nodiscard]] const rttr::type& return_type() const noexcept
        {
            return return_type_;
        }

        [[nodiscard]] const std::vector<Parameter>& parameters() const noexcept
        {
            return parameters_;
        }

        [[nodiscard]] bool is_static() const noexcept
        {
            return is_static_;
        }

        [[nodiscard]] std::string_view category() const noexcept
        {
            return category_;
        }

        [[nodiscard]] std::string_view description() const noexcept
        {
            return description_;
        }

      private:
        rttr::method           method_;
        rttr::type             return_type_;
        std::vector<Parameter> parameters_;
        bool                   is_static_;
        std::string            category_;
        std::string            description_;
    };
} // namespace engine::model