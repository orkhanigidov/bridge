/**
 * Project: Engine
 * Tool: Codegen
 * File: clang_utils.cpp
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
 * @file clang_utils.cpp
 * @brief Implements utility functions for Clang AST and type handling.
 */


#include "analysis/utils/clang_utils.hpp"

#include <algorithm>
#include <sstream>
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
    std::string to_std_string(const CXString& cx_string)
    {
        const char* c_str = clang_getCString(cx_string);
        std::string result = c_str ? c_str : "";
        clang_disposeString(cx_string);
        return result;
    }

    /**
     * @brief Gets the spelling (name) of a Clang cursor.
     * @param cursor The Clang cursor.
     * @return The spelling as a std::string.
     */
    std::string get_spelling(const CXCursor& cursor)
    {
        return to_std_string(clang_getCursorSpelling(cursor));
    }

    /**
     * @brief Gets the spelling of a Clang type.
     * @param type The Clang type.
     * @return The type spelling as a std::string.
     */
    std::string get_type_spelling(const CXType& type)
    {
        // CXType canonical_type = clang_getCanonicalType(type);
        return to_std_string(clang_getTypeSpelling(type));
    }

    /**
     * @brief Gets the type spelling of a Clang cursor.
     * @param cursor The Clang cursor.
     * @return The type spelling as a std::string.
     */
    std::string get_cursor_type_spelling(const CXCursor& cursor)
    {
        return get_type_spelling(clang_getCursorType(cursor));
    }

    /**
     * @brief Gets the result type spelling of a Clang cursor (e.g., function return type).
     * @param cursor The Clang cursor.
     * @return The result types spelling as a std::string.
     */
    std::string get_cursor_result_type_spelling(const CXCursor& cursor)
    {
        return get_type_spelling(clang_getCursorResultType(cursor));
    }

    /**
     * @brief Gets the include path for the file where the cursor is defined.
     * @param cursor The Clang cursor.
     * @return The include path as a std::string.
     */
    std::string get_include_path(const CXCursor& cursor)
    {
        const CXSourceLocation location = clang_getCursorLocation(cursor);
        CXFile file;
        clang_getFileLocation(location, &file, nullptr, nullptr, nullptr);
        if (!file)
        {
            return "";
        }
        std::string path = to_std_string(clang_getFileName(file));
        for (char& c : path)
        {
            if (c == '\\')
            {
                c = '/';
            }
        }
        return path;
    }

    /**
     * @brief Extracts parameter descriptors from a function or method cursor.
     * @param cursor The Clang cursor representing a function or method.
     * @return Vector of ParameterDescriptor objects.
     */
    std::vector<metadata::ParameterDescriptor> get_parameters(const CXCursor& cursor)
    {
        std::vector<metadata::ParameterDescriptor> parameters;
        const int num_args = clang_Cursor_getNumArguments(cursor);
        if (num_args <= 0)
        {
            return parameters;
        }
        parameters.reserve(static_cast<size_t>(num_args));
        for (int i = 0; i < num_args; ++i)
        {
            CXCursor parameter_cursor = clang_Cursor_getArgument(cursor, i);
            parameters.emplace_back(get_spelling(parameter_cursor), get_cursor_type_spelling(parameter_cursor));
        }
        return parameters;
    }

    /**
     * @brief Builds a function signature string from parameter descriptors.
     * @param parameters The vector of ParameterDescriptor objects.
     * @return The function signature as a std::string.
     */
    std::string build_signature(const std::vector<metadata::ParameterDescriptor>& parameters)
    {
        std::ostringstream signature_stream;
        signature_stream << "(";
        for (size_t i = 0; i < parameters.size(); ++i)
        {
            signature_stream << parameters[i].type_name();
            if (i != parameters.size() - 1)
            {
                signature_stream << ", ";
            }
        }
        signature_stream << ")";
        return signature_stream.str();
    }
} // namespace codegen::analysis::utils
