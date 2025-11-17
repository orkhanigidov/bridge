/**
 * Project: Engine
 * Tool: Codegen
 * File: class_descriptor.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file class_descriptor.hpp
 * @brief Declares the ClassDescriptor for code generation metadata.
 */

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
    /**
     * @class ClassDescriptor
     * @brief Describes a C++ class for code generation metadata.
     *
     * Holds information about the class name, template types, base classes, constructors, member enumerators, variables, and functions.
     */
    class ClassDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ClassDescriptor() = default;

        /**
         * @brief Constructs a ClassDescriptor with a given class name.
         * @param name The name of the class.
         */
        explicit ClassDescriptor(std::string name) : name_(std::move(name))
        {
        }

        /**
         * @brief Gets the class name.
         * @return The class name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the template type names.
         * @return The vector of template type names.
         */
        const std::vector<std::string>& template_types() const noexcept
        {
            return template_types_;
        }

        /**
         * @brief Gets the base class names.
         * @return The vector of base class names.
         */
        const std::vector<std::string>& base_class_names() const noexcept
        {
            return base_class_names_;
        }

        /**
         * @brief Gets the class constructors.
         * @return The vector of constructor descriptors.
         */
        const std::vector<ConstructorDescriptor>& constructors() const noexcept
        {
            return constructors_;
        }

        /**
         * @brief Gets the member enumerators (enums).
         * @return The vector of enum descriptors.
         */
        const std::vector<EnumDescriptor>& member_enumerators() const noexcept
        {
            return member_enumerators_;
        }

        /**
         * @brief Gets the member variables.
         * @return The vector of variable descriptors.
         */
        const std::vector<VariableDescriptor>& member_variables() const noexcept
        {
            return member_variables_;
        }

        /**
         * @brief Gets the member functions.
         * @return The vector of function descriptors.
         */
        const std::vector<FunctionDescriptor>& member_functions() const noexcept
        {
            return member_functions_;
        }

        /**
         * @brief Sets the class name.
         * @param name The new class name.
         * @return Reference to this descriptor.
         */
        ClassDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Adds a template type name.
         * @param template_type The template type name.
         * @return Reference to this descriptor.
         */
        ClassDescriptor& add_template_type(std::string template_type)
        {
            template_types_.emplace_back(std::move(template_type));
            return *this;
        }

        /**
         * @brief Adds a base class name.
         * @param base_class_name The base class name.
         * @return Reference to this descriptor.
         */
        ClassDescriptor& add_base_class_name(std::string base_class_name)
        {
            base_class_names_.emplace_back(std::move(base_class_name));
            return *this;
        }

        /**
         * @brief Adds a constructor descriptor.
         * @param constructor The constructor descriptor.
         * @return Reference to this descrioptor.
         */
        ClassDescriptor& add_constructor(ConstructorDescriptor constructor)
        {
            constructors_.emplace_back(std::move(constructor));
            return *this;
        }

        /**
         * @brief Adds a member enumerator (enum).
         * @param enumerator The enum descriptor.
         * @return Reference to this descriptor.
         */
        ClassDescriptor& add_member_enumerator(EnumDescriptor enumerator)
        {
            member_enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

        /**
         * @brief Adds a member variable.
         * @param member_variable The variable descriptor.
         * @return Reference to this desciptor.
         */
        ClassDescriptor& add_member_variable(VariableDescriptor member_variable)
        {
            member_variables_.emplace_back(std::move(member_variable));
            return *this;
        }

        /**
         * @brief Adds a member function.
         * @param member_function The function descriptor.
         * @return Reference to this descriptor.
         */
        ClassDescriptor& add_member_function(FunctionDescriptor member_function)
        {
            member_functions_.emplace_back(std::move(member_function));
            return *this;
        }

    private:
        /**
         * @brief The class name.
         */
        std::string name_;

        /**
         * @brief Template type names.
         */
        std::vector<std::string> template_types_;

        /**
         * @brief Base class names.
         */
        std::vector<std::string> base_class_names_;

        /**
         * @brief Constructors.
         */
        std::vector<ConstructorDescriptor> constructors_;

        /**
         * @brief Member enumerators (enums).
         */
        std::vector<EnumDescriptor> member_enumerators_;

        /**
         * @brief Member variables.
         */
        std::vector<VariableDescriptor> member_variables_;

        /**
         * @brief Member functions.
         */
        std::vector<FunctionDescriptor> member_functions_;
    };
} // namespace codegen::metadata
