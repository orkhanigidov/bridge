#pragma once

#include "analysis_data.hpp"

#include <clang-c/Index.h>

namespace codegen::analysis
{
    class AstVisitor final
    {
    public:
        explicit AstVisitor(const AnalysisConfig& config): config_(config)
        {
        }

        CXChildVisitResult visit(const CXCursor& cursor, const CXCursor& parent);
        static CXChildVisitResult visitor_callback(CXCursor cursor, CXCursor parent, CXClientData client_data);
        static void collect_all_base_cursors(const CXCursor& cursor, std::vector<CXCursor>& bases);

        const AnalysisResult& result() const
        {
            return result_;
        }

    private:
        AnalysisConfig config_;
        AnalysisResult result_;

        void visit_class_decl(const CXCursor& cursor);
        void visit_class_template(const CXCursor& cursor);
        void visit_function_decl(const CXCursor& cursor);
        void visit_enum_decl(const CXCursor& cursor);
        static std::optional<metadata::EnumDescriptor> parse_enum_decl(const CXCursor& cursor);
        static CXChildVisitResult visit_class_member(CXCursor cursor, CXCursor parent, CXClientData client_data);
        void process_class_common(const CXCursor& cursor, metadata::ClassDescriptor& class_desc);
    };
} // namespace codegen::analysis
