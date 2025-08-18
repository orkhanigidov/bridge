#pragma once

#include "constructor_descriptor.hpp"
#include "function_descriptor.hpp"
#include "variable_descriptor.hpp"

namespace metadata {

    class ClassDescriptor final {
    public:
        ClassDescriptor() = default;
        
        explicit ClassDescriptor(std::string name):
            m_name(std::move(name)) {}

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

        const std::vector<VariableDescriptor>& member_variables() const noexcept
        {
            return m_member_variables;
        }

        const std::vector<FunctionDescriptor>& member_functions() const noexcept
        {
            return m_member_functions;
        }

        ClassDescriptor& set_name(std::string name)
        {
            m_name = std::move(name);
            return *this;
        }

        ClassDescriptor& add_base_class_name(std::string base_class)
        {
            m_base_class_names.emplace_back(std::move(base_class));
            return *this;
        }

        ClassDescriptor& add_constructor(ConstructorDescriptor constructor)
        {
            m_constructors.emplace_back(std::move(constructor));
            return *this;
        }

        ClassDescriptor &add_member_variable(VariableDescriptor member_variable)
        {
            m_member_variables.emplace_back(std::move(member_variable));
            return *this;
        }

        ClassDescriptor& add_member_function(FunctionDescriptor member_function)
        {
            m_member_functions.emplace_back(std::move(member_function));
            return *this;
        }

    private:
        std::string m_name;
        std::vector<std::string> m_base_class_names;
        std::vector<ConstructorDescriptor> m_constructors;
        std::vector<VariableDescriptor> m_member_variables;
        std::vector<FunctionDescriptor> m_member_functions;
    };

} // namespace metadata
