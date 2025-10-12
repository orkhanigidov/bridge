#pragma once

#include "scope.hpp"

namespace codegen::metadata
{
    class VariableDescriptor final
    {
    public:
        VariableDescriptor() = default;

        explicit VariableDescriptor(Scope scope, std::string name): scope_(scope), name_(std::move(name))
        {
        }

        Scope scope() const noexcept
        {
            return scope_;
        }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        bool is_static() const noexcept
        {
            return is_static_;
        }

        bool is_const() const noexcept
        {
            return is_const_;
        }

        VariableDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        VariableDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        VariableDescriptor& set_type_name(std::string type_name)
        {
            type_name_ = std::move(type_name);
            return *this;
        }

        VariableDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        VariableDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

    private:
        Scope scope_{Scope::Global};
        std::string name_;
        std::string type_name_;
        bool is_static_{false};
        bool is_const_{false};
    };
} // namespace codegen::metadata
