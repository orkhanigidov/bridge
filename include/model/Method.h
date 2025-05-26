#pragma once

#include "../pch.h"
#include "Parameter.h"

namespace engine::model
{
    class Method final
    {
      public:
        explicit Method(rttr::method method, const rttr::type& returnType, const std::vector<Parameter>& parameters,
                        std::string category = "", std::string description = "", bool isStatic = true);
        ~Method() = default;

        Method(const Method&)            = delete;
        Method& operator=(const Method&) = delete;
        Method(Method&&)                 = default;
        Method& operator=(Method&&)      = default;

        [[nodiscard]] rttr::method getMethod() const noexcept;
        [[nodiscard]] rttr::type getReturnType() const noexcept;
        [[nodiscard]] const std::vector<Parameter>& getParameters() const noexcept;
        [[nodiscard]] std::string getCategory() const noexcept;
        [[nodiscard]] std::string getDescription() const noexcept;
        [[nodiscard]] bool isStatic() const noexcept;

      private:
        rttr::method m_method;
        rttr::type m_returnType;
        std::vector<Parameter> m_parameters;
        std::string m_category;
        std::string m_description;
        bool m_isStatic{true};
    };
} // namespace engine::model