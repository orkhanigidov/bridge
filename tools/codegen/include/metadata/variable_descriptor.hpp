#pragma once

#include "scope.hpp"

namespace codegen::metadata
{
    /**
     * @brief Represents metadata for a variable in the code generation system.
     *
     * This class holds information about a variable's scope, name, type,
     * and qualifiers (static, const). It provides methods to query and modify
     * this metadata in a fluent interface style.
     */
    class VariableDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        VariableDescriptor() = default;

        /**
         * @brief Constructs a VariableDescriptor with the given scope and name.
         * @param scope The scope of the variable.
         * @param name The name of the variable.
         */
        explicit VariableDescriptor(Scope scope, std::string name): scope_(scope),
                                                                    name_(std::move(name))
        {
        }

        /**
         * @brief Gets the scope of the variable.
         * @return The scope of the variable.
         */
        Scope scope() const noexcept
        {
            return scope_;
        }

        /**
         * @brief Gets the name of the variable.
         * @return A constant reference to the variable name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the type name of the variable.
         * @return A constant reference to the variable type name.
         */
        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        /**
         * @brief Checks if the variable is static.
         * @return True if the variable is static, false otherwise.
         */
        bool is_static() const noexcept
        {
            return is_static_;
        }

        /**
         * @brief Checks if the variable is const.
         * @return True if the variable is const, false otherwise.
         */
        bool is_const() const noexcept
        {
            return is_const_;
        }

        /**
         * @brief Sets the scope of the variable.
         * @param scope The new scope of the variable.
         * @return A reference to this VariableDescriptor for chaining.
         */
        VariableDescriptor& set_scope(Scope scope) noexcept
        {
            scope_ = scope;
            return *this;
        }

        /**
         * @brief Sets the name of the variable.
         * @param name The new name of the variable.
         * @return A reference to this VariableDescriptor for chaining.
         */
        VariableDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the type name of the variable.
         * @param type_name The type name of the variable.
         * @return A reference to this VariableDescriptor for chaining.
         */
        VariableDescriptor& set_type_name(std::string type_name)
        {
            type_name_ = std::move(type_name);
            return *this;
        }

        /**
         * @brief Sets whether the variable is static.
         * @param value True to make the variable static, false otherwise.
         * @return A reference to this VariableDescriptor for chaining.
         */
        VariableDescriptor& set_static(bool value) noexcept
        {
            is_static_ = value;
            return *this;
        }

        /**
         * @brief Sets whether the variable is const.
         * @param value True to make the variable const, false otherwise.
         * @return A reference to this VariableDescriptor for chaining.
         */
        VariableDescriptor& set_const(bool value) noexcept
        {
            is_const_ = value;
            return *this;
        }

    private:
        Scope scope_{Scope::Global}; /**< The scope of the variable. Default is Global. */
        std::string name_; /**< The name of the variable. */
        std::string type_name_; /**< The type name of the variable. */
        bool is_static_{false}; /**< Whether the variable is static. Default is false. */
        bool is_const_{false}; /**< Whether the variable is const. Default is false. */
    };
} // namespace codegen::metadata
