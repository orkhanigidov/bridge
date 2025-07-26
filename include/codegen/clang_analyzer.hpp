#pragma once

#include "engine/meta/class_descriptor.hpp"
#include <clang-c/Index.h>

namespace codegen {
    class ClangAnalyzer final {
    public:
        explicit ClangAnalyzer(const std::string& filename, const std::unordered_map<std::string, std::vector<std::string>>& target_classes, const std::vector<std::string>& target_free_functions);

        const std::vector<engine::meta::ClassDescriptor>& found_classes() { return found_classes_; }

        const std::vector<engine::meta::FunctionDescriptor>& found_free_functions() { return found_free_functions_; }

        const std::vector<std::string>& found_includes() { return found_includes_; }

    private:
        std::unordered_map<std::string, std::vector<std::string>> target_classes_;
        std::vector<std::string> target_free_functions_;
        std::vector<engine::meta::ClassDescriptor> found_classes_;
        std::vector<engine::meta::FunctionDescriptor> found_free_functions_;
        std::vector<std::string> found_includes_;

        static std::string get_spelling(const CXCursor& cursor);
        static std::string get_result_type(const CXCursor& cursor);
        static std::string get_type(const CXCursor& cursor);
        static std::string get_qualified_type(const CXCursor& cursor);
        static std::vector<engine::meta::ParameterDescriptor> get_arguments(const CXCursor& cursor);
        static std::string get_include_path(const CXCursor& cursor);
        static CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    };
} // namespace codegen
