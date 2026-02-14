/**
 * Project: Engine
 * Tool: Codegen
 * File: clang_analyzer.hpp
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
 * @file clang_analyzer.hpp
 * @brief Declares the ClangAnalyzer class for analyzing C++ source files using Clang.
 */

#pragma once

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <clang-c/Index.h>

#include "analysis_data.hpp"
#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

namespace codegen::analysis
{
    /**
     * @class ClangAnalyzerException
     * @brief Exception type for ClangAnalyzer errors.
     */
    class ClangAnalyzerException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * @typedef IndexPtr
     * @brief Smart pointer for managing CXIndex resources.
     */
    using IndexPtr = std::unique_ptr<std::remove_pointer_t<CXIndex>, decltype(&clang_disposeIndex)>;

    /**
     * @typedef TranslationUnitPtr
     * @brief Smart pointer for managing CXTranslationUnit resources.
     */
    using TranslationUnitPtr = std::unique_ptr<std::remove_pointer_t<CXTranslationUnit>, decltype(&clang_disposeTranslationUnit)>;

    /**
     * @class ClangAnalyzer
     * @brief Analyzes C++ source files using Clang and collects code metadata.
     *
     * The ClangAnalyzer parses a source file and extracts includes, namespaces, classes, free functions, and enums, according to the provided analysis configuration.
     */
    class ClangAnalyzer final
    {
    public:
        /**
         * @brief Constructs a ClangAnalyzer for the given file and configuration.
         * @param file_path Path to the source file to analyze.
         * @param config Analysis configuration specifying what to extract.
         */
        explicit ClangAnalyzer(const std::filesystem::path& file_path, const AnalysisConfig& config);

        /**
         * @brief Returns the set of found include paths.
         * @return Set of include paths.
         */
        const std::unordered_set<std::string>& found_includes() const noexcept
        {
            return result_.includes;
        }

        /**
         * @brief Returns the set of found namespaces.
         * @return Set of namespaces.
         */
        const std::unordered_set<std::string>& found_namespaces() const noexcept
        {
            return result_.namespaces;
        }

        /**
         * @brief Returns the set of found containers.
         * @return Set of containers.
         */
        const std::unordered_set<std::string>& found_containers() const noexcept
        {
            return result_.containers;
        }

        /**
         * @brief Returns the list of found class descriptors.
         * @return Vector of class descriptors.
         */
        const std::vector<metadata::ClassDescriptor>& found_classes() const noexcept
        {
            return result_.classes;
        }

        /**
         * @brief Returns the list of found free function descriptors.
         * @return Vector of function descriptors.
         */
        const std::vector<metadata::FunctionDescriptor>& found_free_functions() const noexcept
        {
            return result_.free_functions;
        }

        /**
         * @brief Returns the list of found enum descriptors.
         * @return Vector of enum descriptors.
         */
        const std::vector<metadata::EnumDescriptor>& found_enums() const noexcept
        {
            return result_.enums;
        }

        /**
         * @brief Returns the complete analysis result.
         * @return AnalysisResult structure.
         */
        const AnalysisResult& result() const noexcept
        {
            return result_;
        }

    private:
        /**
         * @brief Stores the results of the analysis.
         */
        AnalysisResult result_;
    };
} // namespace codegen::analysis
