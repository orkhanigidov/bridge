#pragma once

#include "parameter_descriptor.hpp"
#include "scope.hpp"

namespace codegen::metadata
{
    /**
     * @brief Represents metadata for a function in the code generation system.
     *
     * This class holds information about a function's scope, name, return type,
     * parameters, qualifiers (static, const) and signature. It provides methods
     * to query and modify this metadata in a fluent interface style.
     */
    class FunctionDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        FunctionDescriptor() = default;

        /**
         * @brief Constructs a FunctionDescriptor with the given scope and name.
         * @param scope The scope of the function.
         * @param name The name of the function.
         */
        explicit FunctionDescriptor(Scope scope, std::string name): scope_(scope),
                                                                    name_(std::move(name))
        {
        }

        /**
         * @brief Gets the scope of the function.
         * @return The scope of the function.
         */
        Scope scope() const noexcept
        {
            return scope_;
        }

        /**
         * @brief Gets the name of the function.
         * @return A constant reference to the function name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the return type name of the function.
         * @return A constant reference to the return type name.
         */
        const std::string& return_type_name() const noexcept
        {
            return return_type_name_;
        }

        /**
         * @brief Gets the list of parameters.
         * @return A constant reference to the vector of ParameterDescriptors.
         */
        const std::vector<ParameterDescriptor>& parameters() const noexcept
        {
            return parameters_;
        }

        /**
         * @brief Checks if the function is static.
         * @return True if the function is static, false otherwise.
         */
        bool is_static() const noexcept
        {
            return is_static_;
        }

        /**
         * @brief Checks if the function is const.
         * @return True if the function is const, false otherwise.
         */
        bool is_const() const noexcept
        {
            return is_const_;
        }

        /**
         * @brief Gets the signature of the function.
         * @return A constant reference to the function signature.
         */
        const std::string& signature() const noexcept
        {
            return signature_;
        }

        /**
         * @brief Sets the scope of the function.
         * @param scope The new scope of the function.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        /**
         * @brief Sets the name of the function.
         * @param name The new name of the function.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the return type name of the function.
         * @param return_type_name The return type name of the function.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_return_type_name(std::string return_type_name)
        {
            return_type_name_ = std::move(return_type_name);
            return *this;
        }

        /**
         * @brief Adds a parameter descriptor.
         * @param parameter The ParameterDescriptor to add.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        /**
         * @brief Sets whether the function is static.
         * @param value True to make the function static, false otherwise.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        /**
         * @brief Sets whether the function is const.
         * @param value True to make the function const, false otherwise.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

        /**
         * @brief Sets the signature of the function.
         * @param signature The signature of the function.
         * @return A reference to this FunctionDescriptor for chaining.
         */
        FunctionDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        Scope scope_{Scope::Global}; /**< The scope of the function. Default is Global. */
        std::string name_; /**< The name of the function. */
        std::string return_type_name_; /**< The return type name of the function. */
        std::vector<ParameterDescriptor> parameters_; /**< The list of parameter descriptors. */
        bool is_static_{false}; /**< Whether the function is static. Default is false. */
        bool is_const_{false}; /**< Whether the function is const. Default is false. */
        std::string signature_; /**< The signature of the function. */
    };
} // namespace codegen::metadata
