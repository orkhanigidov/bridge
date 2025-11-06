/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file memory_ownership.hpp
 * @brief Declares the MemoryOwnership enum for Lua bindings.
 */

#pragma once

namespace engine::bindings::lua
{
    /**
     * @enum MemoryOwnership
     * @brief Specifies the ownership of memory for bound objects.
     *
     * Used to indicate whether memory is managed by C++ or Lua.
     */
    enum class MemoryOwnership
    {
        /**
         * @brief Memory is owned and managed by C++.
         */
        Cpp,

        /**
         * @brief Memory is owned and managed by Lua.
         */
        Lua
    };
} // namespace engine::bindings::lua
