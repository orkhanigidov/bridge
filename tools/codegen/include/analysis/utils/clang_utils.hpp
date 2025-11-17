/**
 * Project: Engine
 * Tool: Codegen
 * File: clang_utils.hpp
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
 * @file clang_utils.hpp
 * @brief Utility functions for Clang AST and type handling.
 */

#pragma once

#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "metadata/parameter_descriptor.hpp"

namespace codegen::analysis::utils
{
    /**
     * @brief Converts a CXString to a std::string and disposes the CXString.
     * @param cx_string The Clang CXString to convert.
     * @return The converted std::string.
     */
    std::string to_std_string(const CXString& cx_string);

    /**
     * @brief Gets the spelling (name) of a Clang cursor.
     * @param cursor The Clang cursor.
     * @return The spelling as a std::string.
     */
    std::string get_spelling(const CXCursor& cursor);

    /**
     * @brief Gets the spelling of a Clang type.
     * @param type The Clang type.
     * @return The type spelling as a std::string.
     */
    std::string get_type_spelling(const CXType& type);

    /**
     * @brief Gets the type spelling of a Clang cursor.
     * @param cursor The Clang cursor.
     * @return The type spelling as a std::string.
     */
    std::string get_cursor_type_spelling(const CXCursor& cursor);

    /**
     * @brief Gets the result type spelling of a Clang cursor (e.g., function return type).
     * @param cursor The Clang cursor.
     * @return The result types spelling as a std::string.
     */
    std::string get_cursor_result_type_spelling(const CXCursor& cursor);

    /**
     * @brief Gets the include path for the file where the cursor is defined.
     * @param cursor The Clang cursor.
     * @return The include path as a std::string.
     */
    std::string get_include_path(const CXCursor& cursor);

    /**
     * @brief Extracts parameter descriptors from a function or method cursor.
     * @param cursor The Clang cursor representing a function or method.
     * @return Vector of ParameterDescriptor objects.
     */
    std::vector<metadata::ParameterDescriptor> get_parameters(const CXCursor& cursor);

    /**
     * @brief Builds a function signature string from parameter descriptors.
     * @param parameters The vector of ParameterDescriptor objects.
     * @return The function signature as a std::string.
     */
    std::string build_signature(const std::vector<metadata::ParameterDescriptor>& parameters);
} // namespace codegen::analysis::utils
