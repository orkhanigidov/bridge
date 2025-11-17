/**
 * Project: Engine
 * Tool: Codegen
 * File: parameter_descriptor.hpp
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
 * @file parameter_descriptor.hpp
 * @brief Declares the ParameterDescriptor for code generation metadata.
 */

#pragma once

#include <string>
#include <utility>

namespace codegen::metadata
{
    /**
     * @class ParameterDescriptor
     * @brief Describes a function or constructor parameter for code generation metadata.
     *
     * Holds information about the parameter's name and type.
     */
    class ParameterDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ParameterDescriptor() = default;

        /**
         * @brief Constructs a ParameterDescriptor with a given name and type.
         * @param name The name of the parameter.
         * @param type_name The type name of the parameter.
         */
        explicit ParameterDescriptor(std::string name, std::string type_name) : name_(std::move(name)),
                                                                                type_name_(std::move(type_name))
        {
        }

        /**
         * @brief Gets the parameter's name.
         * @return The name of the parameter.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the parameter's type name.
         * @return The type name of the parameter.
         */
        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        /**
         * @brief Sets the parameter's name.
         * @param name The new name.
         * @return Reference to this descriptor.
         */
        ParameterDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the parameter's type name.
         * @param type_name The new type name.
         * @return Reference to this descriptor.
         */
        ParameterDescriptor& set_type_name(std::string type_name)
        {
            type_name_ = std::move(type_name);
            return *this;
        }

    private:
        /**
         * @brief The parameter's name.
         */
        std::string name_;

        /**
         * @brief The parameter's type name.
         */
        std::string type_name_;
    };
} // namespace codegen::metadata
