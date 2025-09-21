#pragma once

#include <clang-c/Index.h>
#include <metadata/class_descriptor.hpp>

namespace codegen {

    class ClangAnalyzer final {
    public:
        explicit ClangAnalyzer(const std::string& filename,
                               const std::unordered_map<std::string, std::vector<std::string>>& target_classes,
                               const std::vector<std::string>& target_free_functions);

        const std::vector<metadata::ClassDescriptor>& found_classes()
        {
            return m_found_classes;
        }

        const std::vector<metadata::FunctionDescriptor>& found_free_functions()
        {
            return m_found_free_functions;
        }

        const std::vector<std::string>& found_includes()
        {
            return m_found_includes;
        }

    private:
        std::unordered_map<std::string, std::vector<std::string>> m_target_classes;
        std::vector<std::string> m_target_free_functions;
        std::vector<metadata::ClassDescriptor> m_found_classes;
        std::vector<metadata::FunctionDescriptor> m_found_free_functions;
        std::vector<std::string> m_found_includes;

        static std::string get_spelling(const CXCursor& cursor);
        static std::string get_result_type(const CXCursor& cursor);
        static std::string get_type(const CXCursor& cursor);
        static std::string get_qualified_type(const CXCursor& cursor);
        static std::vector<metadata::ParameterDescriptor> get_arguments(const CXCursor& cursor);
        static std::string get_include_path(const CXCursor& cursor);
        static CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    };

} // namespace codegen
