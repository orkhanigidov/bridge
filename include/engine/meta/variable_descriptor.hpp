#pragma once

#include "pch.hpp"

namespace engine::meta
{
    class VariableDescriptor final
    {
      public:
        VariableDescriptor() = default;

        const std::string& name() const noexcept
        {
            return name_;
        }

        void setName(const std::string& name)
        {
            name_ = name;
        }

        const std::string& type() const noexcept
        {
            return type_;
        }

        void setType(const std::string& type)
        {
            type_ = type;
        }

        bool isStatic() const noexcept
        {
            return is_static_;
        }

        void setStatic(bool is_static)
        {
            is_static_ = is_static;
        }

      private:
        std::string name_;
        std::string type_;
        bool is_static_{false};
    };
} // namespace engine::meta