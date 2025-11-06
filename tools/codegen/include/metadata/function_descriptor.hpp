/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file function_descriptor.hpp
 * @brief Declares the FunctionDescriptor for code generation metadata.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "parameter_descriptor.hpp"
#include "scope.hpp"

namespace codegen::metadata
{
    /**
     * @class FunctionDescriptor
     * @brief Describes a C++ function for code generation metadata.
     *
     * Holds information about the function's scope, name, return type, parameters, and modifiers such as static and const.
     */
    class FunctionDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        FunctionDescriptor() = default;

        /**
         * @brief Constructs a FunctionDescriptor with given scope, name, and return type.
         * @param scope The scope of the function.
         * @param name The name of the function.
         * @param return_type_name The return type name of the function.
         */
        explicit FunctionDescriptor(Scope scope, std::string name, std::string return_type_name) : scope_(scope),
                                                                                                   name_(std::move(name)),
                                                                                                   return_type_name_(std::move(return_type_name))
        {
        }

        /**
         * @brief Gets the function's scope.
         * @return The scope.
         */
        Scope scope() const noexcept
        {
            return scope_;
        }

        /**
         * @brief Gets the function's name.
         * @return The name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the function's return type name.
         * @return The return type name.
         */
        const std::string& return_type_name() const noexcept
        {
            return return_type_name_;
        }

        /**
         * @brief Gets the function's parameters.
         * @return The vector of parameter descriptors.
         */
        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        /**
         * @brief Checks if the function is static.
         * @return True if static, false otherwise.
         */
        bool is_static() const noexcept
        {
            return is_static_;
        }

        /**
         * @brief Checks if the function is const.
         * @return True if const, false otherwise.
         */
        bool is_const() const noexcept
        {
            return is_const_;
        }

        /**
         * @brief Gets the function's signature.
         * @return The signature string.
         */
        const std::string& signature() const noexcept
        {
            return signature_;
        }

        /**
         * @brief Sets the function's scope.
         * @param scope The new scope.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        /**
         * @brief Sets the function's name.
         * @param name The new name.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the function's return type name.
         * @param return_type_name The new return type name.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_return_type_name(std::string return_type_name)
        {
            return_type_name_ = std::move(return_type_name);
            return *this;
        }

        /**
         * @brief Adds a parameter descriptor.
         * @param parameter The parameter descriptor.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        /**
         * @brief Sets whether the function is static.
         * @param value True if static.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        /**
         * @brief Sets whether the function is const.
         * @param value True if const.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

        /**
         * @brief Sets the function's signature.
         * @param signature The new signature string.
         * @return Reference to this descriptor.
         */
        FunctionDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        /**
         * @brief The function's scope.
         */
        Scope scope_{Scope::Global};

        /**
         * @brief The function's name.
         */
        std::string name_;

        /**
         * @brief The function's return type name.
         */
        std::string return_type_name_;

        /**
         * @brief The function's parameters.
         */
        std::vector<ParameterDescriptor> parameters_;

        /**
         * @brief Whether the function is static.
         */
        bool is_static_{false};

        /**
         * @brief Whether the function is const.
         */
        bool is_const_{false};

        /**
         * @brief The function's signature.
         */
        std::string signature_;
    };
} // namespace codegen::metadata
