#pragma once

#include "parameter_descriptor.hpp"

namespace codegen::metadata
{
    /**
     * @brief Represents metadata for a constructor in the code generation system.
     *
     * This class holds information about a constructor's class name, parameters,
     * and signature. It provides methods to query and modify this metadata
     * in a fluent interface style.
     */
    class ConstructorDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ConstructorDescriptor() = default;

        /**
         * @brief Constructs a ConstructorDescriptor with the given class name.
         * @param class_name The name of the class the constructor belongs to.
         */
        explicit ConstructorDescriptor(std::string class_name): class_name_(std::move(class_name))
        {
        }

        /**
         * @brief Gets the name of the class.
         * @return A constant reference to the class name.
         */
        const std::string& class_name() const noexcept
        {
            return class_name_;
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
         * @brief Gets the signature of the constructor.
         * @return A constant reference to the constructor signature.
         */
        const std::string& signature() const noexcept
        {
            return signature_;
        }

        /**
         * @brief Sets the name of the class.
         * @param class_name The new name of the class.
         * @return A reference to this ConstructorDescriptor for chaining.
         */
        ConstructorDescriptor& set_class_name(std::string class_name)
        {
            class_name_ = std::move(class_name);
            return *this;
        }

        /**
         * @brief Adds a parameter descriptor.
         * @param parameter The ParameterDescriptor to add.
         * @return A reference to this ConstructorDescriptor for chaining.
         */
        ConstructorDescriptor& add_parameter(ParameterDescriptor parameter)
        {
            parameters_.emplace_back(std::move(parameter));
            return *this;
        }

        /**
         * @brief Sets the signature of the constructor.
         * @param signature The signature of the constructor to set.
         * @return A reference to this ConstructorDescriptor for chaining.
         */
        ConstructorDescriptor& set_signature(std::string signature)
        {
            signature_ = std::move(signature);
            return *this;
        }

    private:
        std::string class_name_; /**< The name of the class the constructor belongs to. */
        std::vector<ParameterDescriptor> parameters_; /**< List of parameter descriptors. */
        std::string signature_; /**< The signature of the constructor. */
    };
} // namespace codegen::metadata
