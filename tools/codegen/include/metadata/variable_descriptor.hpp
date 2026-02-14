/**
 * Project: Engine
 * Tool: Codegen
 * File: variable_descriptor.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file variable_descriptor.hpp
 * @brief Declares the VariableDescriptor for code generation metadata.
 */

#pragma once

#include <string>
#include <utility>

#include "scope.hpp"

namespace codegen::metadata
{
    /**
     * @class VariableDescriptor
     * @brief Describes a C++ variable for code generation metadata.
     *
     * Holds information about the variable's scope, name, type, and modifiers.
     */
    class VariableDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        VariableDescriptor() = default;

        /**
         * @brief Constructs a VariableDescriptor with given scope, name, and type.
         * @param scope The scope of the variable.
         * @param name The name of the variable.
         * @param type_name The type name of the variable.
         */
        explicit VariableDescriptor(Scope scope, std::string name, std::string type_name) : scope_(scope),
                                                                                            name_(std::move(name)),
                                                                                            type_name_(std::move(type_name))
        {
        }

        /**
         * @brief Gets the variable's scope.
         * @return The scope.
         */
        Scope scope() const noexcept
        {
            return scope_;
        }

        /**
         * @brief Gets the variable's name.
         * @return The name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the variable's type name.
         * @return The type name.
         */
        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        /**
         * @brief Checks if the variable is static.
         * @return True if static, false otherwise.
         */
        bool is_static() const noexcept
        {
            return is_static_;
        }

        /**
         * @brief Checks if the variable is const.
         * @return True if const, false otherwise.
         */
        bool is_const() const noexcept
        {
            return is_const_;
        }

        /**
         * @brief Checks if the variable is a container.
         * @return True if container, false otherwise.
         */
        bool is_container() const noexcept
        {
            return is_container_;
        }

        /**
         * @brief Sets the variable's scope.
         * @param scope The new scope.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        /**
         * @brief Sets the variable's name.
         * @param name The new name.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the variable's type name.
         * @param type_name The new type name.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_type_name(std::string type_name)
        {
            type_name_ = std::move(type_name);
            return *this;
        }

        /**
         * @brief Sets whether the variable is static.
         * @param value True if static.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        /**
         * @brief Sets whether the variable is const.
         * @param value True if const.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

        /**
         * @brief Sets whether the variable is a container.
         * @param value True if container.
         * @return Reference to this descriptor.
         */
        VariableDescriptor& set_container(bool value) noexcept
        {
            is_container_ = value;
            return *this;
        }

    private:
        /**
         * @brief The variable's scope.
         */
        Scope scope_{Scope::Global};

        /**
         * @brief The variable's name.
         */
        std::string name_;

        /**
         * @brief The variable's type name.
         */
        std::string type_name_;

        /**
         * @brief Whether the variable is static.
         */
        bool is_static_{false};

        /**
         * @brief Whether the variable is const.
         */
        bool is_const_{false};

        /**
         * @brief Whether the variable is a container.
         */
        bool is_container_{false};
    };
} // namespace codegen::metadata
