#pragma once

#include "../pch.h"

namespace engine::model
{
    class Parameter final
    {
      public:
        explicit Parameter(std::string name, const rttr::type& type, rttr::variant defaultValue);
        ~Parameter() = default;

        Parameter(const Parameter&)            = delete;
        Parameter& operator=(const Parameter&) = delete;
        Parameter(Parameter&&)                 = default;
        Parameter& operator=(Parameter&&)      = default;

        [[nodiscard]] const std::string& getName() const noexcept;
        [[nodiscard]] const rttr::type& getType() const noexcept;
        [[nodiscard]] const rttr::variant& getDefaultValue() const noexcept;

      private:
        std::string m_name;
        rttr::type m_type;
        rttr::variant m_defaultValue;
    };
} // namespace engine::model