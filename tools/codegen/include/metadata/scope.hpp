/**
 * Project: Engine
 * Tool: Codegen
 * File: scope.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
