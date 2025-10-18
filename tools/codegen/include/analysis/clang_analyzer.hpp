#pragma once

#include "analysis_data.hpp"

#include <clang-c/Index.h>

namespace codegen::analysis
{
    using IndexPtr = std::unique_ptr<std::remove_pointer_t<CXIndex>, decltype(&clang_disposeIndex)>;
    using TranslationUnitPtr = std::unique_ptr<std::remove_pointer_t<CXTranslationUnit>, decltype(&clang_disposeTranslationUnit)>;

    class ClangAnalyzer final
    {
    public:
        explicit ClangAnalyzer(const std::string& filename, const AnalysisConfig& config);

        const std::unordered_set<std::string>& found_includes() const
        {
            return result_.includes;
        }

        const std::unordered_set<std::string>& found_namespaces() const
        {
            return result_.namespaces;
        }

        const std::vector<metadata::ClassDescriptor>& found_classes() const
        {
            return result_.classes;
        }

        const std::vector<metadata::FunctionDescriptor>& found_free_functions() const
        {
            return result_.free_functions;
        }

        const std::vector<metadata::EnumeratorDescriptor>& found_enums() const
        {
            return result_.enums;
        }

        const AnalysisResult& result() const
        {
            return result_;
        }

    private:
        AnalysisResult result_;
    };
} // namespace codegen::analysis
