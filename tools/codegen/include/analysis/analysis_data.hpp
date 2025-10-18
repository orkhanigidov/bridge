#pragma once

#include "metadata/class_descriptor.hpp"
#include "metadata/function_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"

namespace codegen::analysis
{
    struct ClassConfig
    {
        std::vector<std::string> methods;
        std::vector<std::string> types; // For template classes
    };

    struct AnalysisConfig
    {
        std::string target_include_path;
        std::string wrapper_include_path;
        std::unordered_map<std::string, ClassConfig> target_classes;
        std::vector<std::string> target_free_functions;
    };

    struct AnalysisResult
    {
        std::unordered_set<std::string> includes;
        std::vector<metadata::ClassDescriptor> classes;
        std::vector<metadata::FunctionDescriptor> free_functions;
        std::vector<metadata::EnumDescriptor> enums;
        std::unordered_set<std::string> processed_classes;
    };
} // namespace codegen::analysis
