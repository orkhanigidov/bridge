#pragma once

#include "constructor_descriptor.hpp"
#include "function_descriptor.hpp"
#include "variable_descriptor.hpp"

namespace engine::meta
{
    class ClassDescriptor final
    {
    public:
        ClassDescriptor() = default;

        const std::string& name() const noexcept
        {
            return m_name;
        }

        const std::vector<std::string>& base_class_names() const noexcept
        {
            return m_base_class_names;
        }

        const std::vector<ConstructorDescriptor>& constructors() const noexcept
        {
            return m_constructors;
        }

        const std::vector<VariableDescriptor>& variables() const noexcept
        {
            return m_variables;
        }

        const std::vector<FunctionDescriptor>& methods() const noexcept
        {
            return m_methods;
        }

        void set_name(const std::string& name) noexcept
        {
            m_name = name;
        }

        void add_base_class_name(const std::string& base_class) noexcept
        {
            m_base_class_names.emplace_back(base_class);
        }

        void add_constructor(const ConstructorDescriptor& constructor) noexcept
        {
            m_constructors.emplace_back(constructor);
        }

        void add_variable(const VariableDescriptor& variable) noexcept
        {
            m_variables.emplace_back(variable);
        }

        void add_method(const FunctionDescriptor& method) noexcept
        {
            m_methods.emplace_back(method);
        }

    private:
        std::string m_name;
        std::vector<std::string> m_base_class_names;
        std::vector<ConstructorDescriptor> m_constructors;
        std::vector<FunctionDescriptor> m_methods;
        std::vector<VariableDescriptor> m_variables;
    };
} // namespace engine::meta
