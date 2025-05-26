#pragma once

#include "../pch.h"

namespace engine::model
{
    enum class ParameterType
    {
        Literal,
        Reference,
    };

    class Parameter final
    {
      public:
        explicit Parameter(std::string name, const rttr::type& type, rttr::variant defaultValue, bool isReference);
        ~Parameter() = default;

        Parameter(const Parameter&)                = delete;
        Parameter& operator=(const Parameter&)     = delete;
        Parameter(Parameter&&) noexcept            = default;
        Parameter& operator=(Parameter&&) noexcept = default;

        [[nodiscard]] std::string getName() const noexcept;
        [[nodiscard]] rttr::type getType() const noexcept;
        [[nodiscard]] rttr::variant getDefaultValue() const noexcept;
        [[nodiscard]] bool isReference() const noexcept;

      private:
        std::string m_name;
        rttr::type m_type;
        rttr::variant m_defaultValue;
        bool m_isReference;
    };
} // namespace engine::model