/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file enum_descriptor.hpp
 * @brief Declares the EnumDescriptor for code generation metadata.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

namespace codegen::metadata
{
    /**
     * @struct Enumerator
     * @brief Represents a single enumerator in an enum, with a name and value.
     */
    struct Enumerator
    {
        /**
         * @brief The name of the enumerator.
         */
        std::string name;

        /**
         * @brief The value of the enumerator.
         */
        long long value;
    };

    /**
     * @class EnumDescriptor
     * @brief Describes a C++ enum for code generation metadata.
     *
     * Holds information about the enum name and its enumerators.
     */
    class EnumDescriptor final
    {
    public:
        /**
         * @brief Default constructor.
         */
        EnumDescriptor() = default;

        /**
         * @brief Constructs an EnumDescriptor with a given enum name.
         * @param name The name of the enum.
         */
        explicit EnumDescriptor(std::string name) : name_(std::move(name))
        {
        }

        /**
         * @brief Gets the enum name.
         * @return The enum name.
         */
        const std::string& name() const noexcept
        {
            return name_;
        }

        /**
         * @brief Gets the list of enumerators.
         * @return The vector of enumerators.
         */
        const std::vector<Enumerator>& enumerators() const noexcept
        {
            return enumerators_;
        }

        /**
         * @brief Sets the enum name.
         * @param name The new enum name.
         * @return Reference to this descriptor.
         */
        EnumDescriptor& set_name(std::string name)
        {
            name_ = std::move(name);
            return *this;
        }

        /**
         * @brief Adds an enumerator to the enum.
         * @param enumerator The enumerator to add.
         * @return Reference to this descriptor.
         */
        EnumDescriptor& add_enumerator(Enumerator enumerator)
        {
            enumerators_.emplace_back(std::move(enumerator));
            return *this;
        }

    private:
        /**
         * @brief The enum name.
         */
        std::string name_;

        /**
         * @brief The list of enumerators.
         */
        std::vector<Enumerator> enumerators_;
    };
} // codegen::metadata
