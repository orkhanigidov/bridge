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
    class ClangAnalyzerException final : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    using IndexPtr = std::unique_ptr<std::remove_pointer_t<CXIndex>, decltype(&clang_disposeIndex)>;
    using TranslationUnitPtr = std::unique_ptr<std::remove_pointer_t<CXTranslationUnit>, decltype(&clang_disposeTranslationUnit)>;

    class ClangAnalyzer final
    {
    public:
        explicit ClangAnalyzer(const std::filesystem::path& file_path, const AnalysisConfig& config);

        const std::unordered_set<std::string>& found_includes() const noexcept
        {
            return result_.includes;
        }

        const std::unordered_set<std::string>& found_namespaces() const noexcept
        {
            return result_.namespaces;
        }

        const std::vector<metadata::ClassDescriptor>& found_classes() const noexcept
        {
            return result_.classes;
        }

        const std::vector<metadata::FunctionDescriptor>& found_free_functions() const noexcept
        {
            return result_.free_functions;
        }

        const std::vector<metadata::EnumDescriptor>& found_enums() const noexcept
        {
            return result_.enums;
        }

        const AnalysisResult& result() const noexcept
        {
            return result_;
        }

    private:
        AnalysisResult result_;
    };
} // namespace codegen::analysis
