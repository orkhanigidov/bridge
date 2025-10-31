/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
    std::string to_std_string(const CXString& cx_string)
    {
        const char* c_str = clang_getCString(cx_string);
        std::string result = c_str ? c_str : "";
        clang_disposeString(cx_string);
        return result;
    }

    std::string get_spelling(const CXCursor& cursor)
    {
        return to_std_string(clang_getCursorSpelling(cursor));
    }

    std::string get_type_spelling(const CXType& type)
    {
        CXType canonical_type = clang_getCanonicalType(type);
        return to_std_string(clang_getTypeSpelling(canonical_type));
    }

    std::string get_cursor_type_spelling(const CXCursor& cursor)
    {
        return get_type_spelling(clang_getCursorType(cursor));
    }

    std::string get_cursor_result_type_spelling(const CXCursor& cursor)
    {
        return get_type_spelling(clang_getCursorResultType(cursor));
    }

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
        std::ranges::replace(path, '\\', '/');
        return path;
    }

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
