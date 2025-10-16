#include "analysis/clang_analyzer.hpp"

#include "analysis/ast_visitor.hpp"

namespace codegen::analysis
{
    ClangAnalyzer::ClangAnalyzer(const std::string& filename, const AnalysisConfig& config)
    {
        const IndexPtr index(clang_createIndex(0, 0), &clang_disposeIndex);
        if (!index)
        {
            throw std::runtime_error("Failed to create Clang index.");
        }

        const std::vector<std::string> clang_args = {"-x", "c++", "-std=c++20", "-I" + config.target_include_path};

        std::vector<const char*> clang_args_c;
        for (const auto& arg : clang_args)
        {
            clang_args_c.emplace_back(arg.c_str());
        }

        const TranslationUnitPtr tu(clang_parseTranslationUnit(index.get(), filename.c_str(), clang_args_c.data(),
                                                               clang_args_c.size(), nullptr, 0,
                                                               CXTranslationUnit_None), &clang_disposeTranslationUnit);

        if (!tu)
        {
            throw std::runtime_error("Failed to parse translation unit for file: " + filename);
        }

        AstVisitor visitor(config);

        const CXCursor cursor = clang_getTranslationUnitCursor(tu.get());
        clang_visitChildren(cursor, &AstVisitor::visitor_callback, &visitor);

        result_ = visitor.result();
    }
} // namespace codegen::analysis
