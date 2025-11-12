/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file clang_analyzer.cpp
 * @brief Implements the ClangAnalyzer class for analyzing C++ source files using Clang.
 */

#include "analysis/clang_analyzer.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include <clang-c/Index.h>

#include "analysis/ast_visitor.hpp"

namespace codegen::analysis
{
    /**
     * @brief Constructs a ClangAnalyzer, parses the given source file, and collects code metadata.
     * @param file_path Path to the C++ source file to analyze.
     * @param config Analysis configuration specifying includes paths and extraction options.
     * @throws ClangAnalyzerException if Clang index or translation unit creation fails.
     */
    ClangAnalyzer::ClangAnalyzer(const std::filesystem::path& file_path, const AnalysisConfig& config)
    {
        const IndexPtr index(clang_createIndex(0, 0), &clang_disposeIndex);
        if (!index)
        {
            throw ClangAnalyzerException("Failed to create Clang index.");
        }

        const std::vector<std::string> clang_args = {
            "-x", "c++",
            "-std=c++17",
            "-I" + config.wrapper_include_path.string(),
            "-I" + config.target_include_path.string()
        };

        std::vector<const char*> clang_args_c;
        clang_args_c.reserve(clang_args.size());
        for (const auto& arg : clang_args)
        {
            clang_args_c.emplace_back(arg.c_str());
        }

        const TranslationUnitPtr tu(clang_parseTranslationUnit(index.get(), file_path.string().c_str(), clang_args_c.data(),
                                                               static_cast<int>(clang_args_c.size()), nullptr, 0,
                                                               CXTranslationUnit_None), &clang_disposeTranslationUnit);

        if (!tu)
        {
            throw ClangAnalyzerException("Failed to parse translation unit for file: " + file_path.string());
        }

        AstVisitor visitor(config);
        const CXCursor cursor = clang_getTranslationUnitCursor(tu.get());
        clang_visitChildren(cursor, &AstVisitor::visitor_callback, &visitor);

        result_ = visitor.result();
    }
} // namespace codegen::analysis
