/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "metadata/parameter_descriptor.hpp"

namespace codegen::analysis::utils
{
    std::string to_std_string(const CXString& cx_string);
    std::string get_spelling(const CXCursor& cursor);
    std::string get_type_spelling(const CXType& type);
    std::string get_cursor_type_spelling(const CXCursor& cursor);
    std::string get_cursor_result_type_spelling(const CXCursor& cursor);
    std::string get_include_path(const CXCursor& cursor);
    std::vector<metadata::ParameterDescriptor> get_parameters(const CXCursor& cursor);
    std::string build_signature(const std::vector<metadata::ParameterDescriptor>& parameters);
} // namespace codegen::analysis::utils
