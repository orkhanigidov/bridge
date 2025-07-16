#pragma once

#include "function_descriptor.hpp"
#include "pch.hpp"
#include "variable_descriptor.hpp"

namespace engine::meta
{
    class ClassDescriptor final
    {
      public:
        ClassDescriptor() = default;

        const std::string& name() const noexcept
        {
            return name_;
        }

        void setName(const std::string& name)
        {
            name_ = name;
        }

        const std::vector<FunctionDescriptor>& constructors() const noexcept
        {
            return constructors_;
        }

        void addConstructor(const FunctionDescriptor& constructor)
        {
            constructors_.emplace_back(constructor);
        }

        const std::vector<std::string>& baseClasses() const noexcept
        {
            return base_classes_;
        }

        void addBaseClass(const std::string& base_class)
        {
            base_classes_.emplace_back(base_class);
        }

        const std::vector<FunctionDescriptor>& methods() const noexcept
        {
            return methods_;
        }

        void addMethod(const FunctionDescriptor& method)
        {
            methods_.emplace_back(method);
        }

        const std::vector<VariableDescriptor>& variables() const noexcept
        {
            return variables_;
        }

        void addVariable(const VariableDescriptor& variable)
        {
            variables_.emplace_back(variable);
        }

      private:
        std::string name_;
        std::vector<FunctionDescriptor> constructors_;
        std::vector<std::string> base_classes_;
        std::vector<FunctionDescriptor> methods_;
        std::vector<VariableDescriptor> variables_;
    };
} // namespace engine::meta