/**
 * Project: Engine
 * Tool: Codegen
 * File: analysis_data.hpp
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
 * @file analysis_data.hpp
 * @brief Data structures for analysis configuration and results.
 */

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

namespace codegen::analysis
{
    /**
     * @struct ClassConfig
     * @brief Configuration for a class to be analyzed.
     *
     * Contains the list of method names and template types to process for a class.
     */
    struct ClassConfig
    {
        /**
         * @brief Names of methods to analyze.
         */
        std::vector<std::string> methods;

        /**
         * @brief Template types for template classes.
         */
        std::vector<std::string> types;
    };

    /**
     * @struct AnalysisConfig
     * @brief Configuration for the analysis process.
     *
     * Specifies include paths, target classes, free functions, and enums to analyze.
     */
    struct AnalysisConfig
    {
        /**
         * @brief The path to the target includes a directory.
         */
        std::filesystem::path target_include_path;

        /**
         * @brief The path to the wrapper includes a directory.
         */
        std::filesystem::path wrapper_include_path;

        /**
         * @brief Classes to analyze.
         */
        std::unordered_map<std::string, ClassConfig> target_classes;

        /**
         * @brief Free functions to analyze.
         */
        std::vector<std::string> target_free_functions;

        /**
         * @brief Enums to analyze.
         */
        std::vector<std::string> target_enums;
    };

    /**
     * @struct AnalysisResult
     * @brief Result of the analysis process.
     *
     * Contains discovered includes, namespaces, classes, free functions, enums, and processed classes.
     */
    struct AnalysisResult
    {
        /**
         * @brief Set of include paths found.Set of include paths found.
         */
        std::unordered_set<std::string> includes;

        /**
         * @brief Set of namespaces found.
         */
        std::unordered_set<std::string> namespaces;

        /**
         * @brief Set of containers found.
         */
        std::unordered_set<std::string> containers;

        /**
         * @brief Analyzed class descriptors.
         */
        std::vector<metadata::ClassDescriptor> classes;

        /**
         * @brief Analyzed free function descriptors.
         */
        std::vector<metadata::FunctionDescriptor> free_functions;

        /**
         * @brief Analyzed enum descriptors.
         */
        std::vector<metadata::EnumDescriptor> enums;

        /**
         * @brief Set of processed class names.
         */
        std::unordered_set<std::string> processed_classes;
    };
} // namespace codegen::analysis
