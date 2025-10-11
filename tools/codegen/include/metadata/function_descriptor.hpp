#pragma once

#include "parameter_descriptor.hpp"
#include "scope.hpp"

namespace engine::metadata
{
    class FunctionDescriptor final
    {
    public:
        FunctionDescriptor() = default;

        explicit FunctionDescriptor(Scope scope, std::string name): scope_(scope), name_(std::move(name))
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

        const std::string& return_type_name() const noexcept
        {
            return return_type_name_;
        }

        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        bool is_static() const noexcept
        {
            return is_static_;
        }

        bool is_const() const noexcept
        {
            return is_const_;
        }

        const std::string& signature() const noexcept
        {
            return signature_;
        }

        FunctionDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        FunctionDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        FunctionDescriptor& set_return_type_name(std::string type)
        {
            return_type_name_ = std::move(type);
            return *this;
        }

        FunctionDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        FunctionDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        FunctionDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

        FunctionDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        Scope scope_{Scope::Global};
        std::string name_;
        std::string return_type_name_;
        std::vector<ParameterDescriptor> parameters_;
        bool is_static_{false};
        bool is_const_{false};
        std::string signature_;
    };
} // namespace engine::metadata
