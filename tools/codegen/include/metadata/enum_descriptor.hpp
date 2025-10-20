#pragma once

namespace codegen::metadata
{
    /**
     * @brief Represents a single enumerator in an enumeration.
     *
     * This struct holds the name and value of an enumerator.
     */
    struct Enumerator
    {
        std::string name; /**< The name of the enumerator. */
        long long value; /**< The value of the enumerator. */
    };

    /**
     * @brief Represents metadata for an enumerator in the code generation system.
     *
     * This class holds information about an enumerator's name and its list of enumerators.
     * It provides methods to query and modify this metadata in a fluent interface style.
     */
    class EnumDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        EnumDescriptor() = default;

        /**
         * @brief Constructs an EnumDescriptor with the given name.
         * @param name The name of the enumerator (e.g., enum name).
         */
        explicit EnumDescriptor(std::string name) : name_(std::move(name))
        {
        }

        /**
         * @brief Gets the name of the enumerator.
         * @return A constant reference to the enumerator name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the list of enumerators.
         * @return A constant reference to the vector of enumerators.
         */
        const std::vector<Enumerator>& enumerators() const noexcept
        {
            return enumerators_;
        }

        /**
         * @brief Sets the name of the enumerator.
         * @param name The name of the enumerator.
         * @return A reference to this EnumDescriptor for chaining.
         */
        EnumDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Adds an enumerator.
         * @param enumerator The enumerator (e.g., name and value).
         * @return A reference to this EnumDescriptor for chaining.
         */
        EnumDescriptor& add_enumerator(Enumerator enumerator)
        {
            enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

    private:
        std::string name_; /**< The name of the enumerator. */
        std::vector<Enumerator> enumerators_; /**< List of enumerators. */
    };
} // codegen::metadata
