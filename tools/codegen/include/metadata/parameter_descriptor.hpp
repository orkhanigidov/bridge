#pragma once

namespace codegen::metadata
{
    /**
     * @brief Represents metadata for a parameter in the code generation system.
     *
     * This class holds information about a parameter's name and type.
     * It provides methods to query and modify this metadata in a fluent interface style.
     */
    class ParameterDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        ParameterDescriptor() = default;

        /**
         * @brief Constructs a ParameterDescriptor with the given name.
         * @param name The name of the parameter.
         */
        explicit ParameterDescriptor(std::string name): name_(std::move(name))
        {
        }

        /**
         * @brief Gets the name of the parameter.
         * @return A constant reference to the parameter name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the type name of the parameter.
         * @return A constant reference to the type name.
         */
        const std::string& type_name() const noexcept
        {
            return type_name_;
        }

        /**
         * @brief Sets the name of the parameter.
         * @param name The new name of the parameter.
         * @return A reference to this ParameterDescriptor for chaining.
         */
        ParameterDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Sets the type name of the parameter.
         * @param type The new type name of the parameter.
         * @return A reference to this ParameterDescriptor for chaining.
         */
        ParameterDescriptor& set_type_name(std::string type)
        {
            type_name_ = std::move(type);
            return *this;
        }

    private:
        std::string name_; /**< The name of the parameter. */
        std::string type_name_; /**< The type name of the parameter. */
    };
} // namespace codegen::metadata
