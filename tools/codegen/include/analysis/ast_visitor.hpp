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

        CXChildVisitResult visit(CXCursor cursor, CXCursor parent);
        static CXChildVisitResult visitor_callback(CXCursor cursor, CXCursor parent, CXClientData client_data);

        const AnalysisResult& result() const
        {
            return result_;
        }

    private:
        AnalysisConfig config_;
        AnalysisResult result_;

        void visit_class_decl(CXCursor cursor);
        void visit_function_decl(CXCursor cursor);
        void visit_enum_decl(CXCursor cursor);
        static CXChildVisitResult visit_class_member(CXCursor cursor, CXCursor parent, CXClientData client_data);
    };
} // namespace codegen::analysis
