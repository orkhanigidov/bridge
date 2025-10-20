#pragma once

#include "constructor_descriptor.hpp"
#include "enum_descriptor.hpp"
#include "function_descriptor.hpp"
#include "variable_descriptor.hpp"

namespace codegen::metadata
{
    /**
     * @brief Represents metadata for a class in the code generation system.
     *
     * This class holds information about a class's name, base classes, constructors,
     * member enumerators, variables, and functions. It provides methods to query
     * and modify this metadata in a fluent interface style.
     */
    class ClassDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ClassDescriptor() = default;

        /**
         * @brief Constructs a ClassDescriptor with the given name.
         * @param name The name of the class.
         */
        explicit ClassDescriptor(std::string name): name_(std::move(name))
        {
        }

        /**
         * @brief Gets the name of the class.
         * @return A constant reference to the class name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the list of base class names.
         * @return A constant reference to the vector of base class names.
         */
        const std::vector<std::string>& base_class_names() const noexcept
        {
            return base_class_names_;
        }

        /**
         * @brief Gets the list of constructors.
         * @return A constant reference to the vector of ConstructorDescriptors.
         */
        const std::vector<ConstructorDescriptor>& constructors() const noexcept
        {
            return constructors_;
        }

        /**
         * @brief Gets the list of member enumerators.
         * @return A constant reference to the vector of EnumDescriptors.
         */
        const std::vector<EnumDescriptor>& member_enumerator() const noexcept
        {
            return member_enumerators_;
        }

        /**
         * @brief Gets the list of member variables.
         * @return A constant reference to the vector of VariableDescriptors.
         */
        const std::vector<VariableDescriptor>& member_variables() const noexcept
        {
            return member_variables_;
        }

        /**
         * @brief Gets the list of member functions.
         * @return A constant reference to the vector of FunctionDescriptors.
         */
        const std::vector<FunctionDescriptor>& member_functions() const noexcept
        {
            return member_functions_;
        }

        /**
         * @brief Sets the name of the class.
         * @param name The name of the class.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Adds a base class name.
         * @param base_class_name The name of the base class.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& add_base_class_name(std::string base_class_name)
        {
            base_class_names_.emplace_back(std::move(base_class_name));
            return *this;
        }

        /**
         * @brief Adds a constructor descriptor.
         * @param constructor The ConstructorDescriptor.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& add_constructor(ConstructorDescriptor constructor)
        {
            constructors_.emplace_back(std::move(constructor));
            return *this;
        }

        /**
         * @brief Adds a member enumerator descriptor.
         * @param enumerator The EnumDescriptor.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& add_member_enum(EnumDescriptor enumerator)
        {
            member_enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

        /**
         * @brief Adds a member variable descriptor.
         * @param member_variable The VariableDescriptor.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& add_member_variable(VariableDescriptor member_variable)
        {
            member_variables_.emplace_back(std::move(member_variable));
            return *this;
        }

        /**
         * @brief Adds a member function descriptor.
         * @param member_function The FunctionDescriptor.
         * @return A reference to this ClassDescriptor for chaining.
         */
        ClassDescriptor& add_member_function(FunctionDescriptor member_function)
        {
            member_functions_.emplace_back(std::move(member_function));
            return *this;
        }

    private:
        std::string name_; /**< The name of the class. */
        std::vector<std::string> base_class_names_; /**< List of base class names. */
        std::vector<ConstructorDescriptor> constructors_; /**< List of constructor descriptors. */
        std::vector<EnumDescriptor> member_enumerators_; /**< List of member enumerator descriptors. */
        std::vector<VariableDescriptor> member_variables_; /**< List of member variable descriptors. */
        std::vector<FunctionDescriptor> member_functions_; /**< List of member function descriptors. */
    };
} // namespace codegen::metadata
