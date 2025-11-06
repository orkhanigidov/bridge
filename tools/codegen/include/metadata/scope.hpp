/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file scope.hpp
 * @brief Declares the Scope enum for code generation metadata.
 */

#pragma once

namespace codegen::metadata
{
    /**
     * @enum Scope
     * @brief Represents the scope of a code element in code generation metadata.
     */
    enum class Scope
    {
        /**
         * @brief Global scope.
         */
        Global,

        /**
         * @brief Member scope.
         */
        Member
    };
} // namespace codegen::metadata
