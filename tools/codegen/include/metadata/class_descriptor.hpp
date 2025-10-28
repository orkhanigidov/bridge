#pragma once

#include <string>
#include <utility>
#include <vector>

#include "constructor_descriptor.hpp"
#include "enum_descriptor.hpp"
#include "function_descriptor.hpp"
#include "variable_descriptor.hpp"

namespace codegen::metadata
{
    class ClassDescriptor final
    {
    public:
        ClassDescriptor() = default;

        explicit ClassDescriptor(std::string name): name_(std::move(name))
        {
        }

        const std::string& name() const noexcept
        {
            return name_;
        }

        const std::vector<std::string>& template_types() const noexcept
        {
            return template_types_;
        }

        const std::vector<std::string>& base_class_names() const noexcept
        {
            return base_class_names_;
        }

        const std::vector<ConstructorDescriptor>& constructors() const noexcept
        {
            return constructors_;
        }

        const std::vector<EnumDescriptor>& member_enumerators() const noexcept
        {
            return member_enumerators_;
        }

        const std::vector<VariableDescriptor>& member_variables() const noexcept
        {
            return member_variables_;
        }

        const std::vector<FunctionDescriptor>& member_functions() const noexcept
        {
            return member_functions_;
        }

        ClassDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        ClassDescriptor& add_template_type(std::string template_type)
        {
            template_types_.emplace_back(std::move(template_type));
            return *this;
        }

        ClassDescriptor& add_base_class_name(std::string base_class_name)
        {
            base_class_names_.emplace_back(std::move(base_class_name));
            return *this;
        }

        ClassDescriptor& add_constructor(ConstructorDescriptor constructor)
        {
            constructors_.emplace_back(std::move(constructor));
            return *this;
        }

        ClassDescriptor& add_member_enumerator(EnumDescriptor enumerator)
        {
            member_enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

        ClassDescriptor& add_member_variable(VariableDescriptor member_variable)
        {
            member_variables_.emplace_back(std::move(member_variable));
            return *this;
        }

        ClassDescriptor& add_member_function(FunctionDescriptor member_function)
        {
            member_functions_.emplace_back(std::move(member_function));
            return *this;
        }

    private:
        std::string name_;
        std::vector<std::string> template_types_;
        std::vector<std::string> base_class_names_;
        std::vector<ConstructorDescriptor> constructors_;
        std::vector<EnumDescriptor> member_enumerators_;
        std::vector<VariableDescriptor> member_variables_;
        std::vector<FunctionDescriptor> member_functions_;
    };
} // namespace codegen::metadata
