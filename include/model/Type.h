#pragma once

#include "../pch.h"

namespace engine::model
{
    class Type final
    {
      public:
        explicit Type(std::string id, std::string name);
        ~Type() = default;

        Type(const Type&)            = delete;
        Type& operator=(const Type&) = delete;
        Type(Type&&)                 = default;
        Type& operator=(Type&&)      = default;

        [[nodiscard]] const std::string& getId() const noexcept;
        [[nodiscard]] const std::string& getName() const noexcept;

      private:
        std::string m_id;
        std::string m_name;
    };
} // namespace engine::model