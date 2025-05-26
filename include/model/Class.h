#pragma once

#include "../pch.h"

namespace engine::model
{
    class Class final
    {
      public:
        explicit Class(std::string id, const rttr::type& type);
        ~Class() = default;

        Class(const Class&)            = delete;
        Class& operator=(const Class&) = delete;
        Class(Class&&)                 = default;
        Class& operator=(Class&&)      = default;

        [[nodiscard]] std::string getId() const noexcept;
        [[nodiscard]] rttr::type getType() const noexcept;

      private:
        std::string m_id;
        rttr::type m_type;
    };
} // namespace engine::model