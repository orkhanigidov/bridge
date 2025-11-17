/**
 * Project: Engine
 * File: memory_ownership.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
