/**
 * Project: Engine
 * Tool: Codegen
 * File: constructor_descriptor.hpp
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
 * @file constructor_descriptor.hpp
 * @brief Declares the ConstructorDescriptor for code generation metadata.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "parameter_descriptor.hpp"

namespace codegen::metadata
{
    /**
     * @class ConstructorDescriptor
     * @brief Describes a C++ constructor for code generation metadata.
     *
     * Holds information about the class name, constructor parameters, and signature.
     */
    class ConstructorDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ConstructorDescriptor() = default;

        /**
         * @brief Constructs a ConstructorDescriptor with a given class name.
         * @param class_name The name of the class.
         */
        explicit ConstructorDescriptor(std::string class_name) : class_name_(std::move(class_name))
        {
        }

        /**
         * @brief Gets the class name.
         * @return The class name.
         */
        const std::string& class_name() const noexcept
        {
            return class_name_;
        }

        /**
         * @brief Gets the constructor parameters.
         * @return The vector of parameter descriptors.
         */
        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        /**
         * @brief Gets the constructor signature.
         * @return The signature string.
         */
        const std::string& signature() const noexcept
        {
            return signature_;
        }

        /**
         * @brief Sets the class name.
         * @param class_name The new class name.
         * @return Reference to this descriptor.
         */
        ConstructorDescriptor& set_class_name(std::string class_name)
        {
            class_name_ = std::move(class_name);
            return *this;
        }

        /**
         * @brief Adds a parameter descriptor.
         * @param parameter The parameter descriptor.
         * @return Reference to this descriptor.
         */
        ConstructorDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        /**
         * @brief Sets the constructor signature.
         * @param signature The new signature string.
         * @return Reference to this descriptor.
         */
        ConstructorDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        /**
         * @brief The class name.
         */
        std::string class_name_;

        /**
         * @brief Constructor parameters.
         */
        std::vector<ParameterDescriptor> parameters_;

        /**
         * @brief Constructor signature.
         */
        std::string signature_;
    };
} // namespace codegen::metadata
