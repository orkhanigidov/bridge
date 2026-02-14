/**
 * Project: Engine
 * Tool: Codegen
 * File: ast_visitor.hpp
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
 * @file ast_visitor.hpp
 * @brief Declares the AstVisitor class for traversing Clang ASTs.
 */

#pragma once

#include <optional>
#include <utility>
#include <vector>
#include <clang-c/Index.h>

#include "analysis_data.hpp"
#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"

namespace codegen::analysis
{
    /**
     * @class AstVisitor
     * @brief Visits and analyzes Clang AST nodes to collect code metadata.
     *
     * The AstVisitor traverses the Clang AST, collecting information about classes, functions, and enums as specified in the analysis configuration.
     */
    class AstVisitor final
    {
    public:
        /**
         * @brief Constructs an AstVisitor with the given analysis configuration.
         * @param config The analysis configuration.
         */
        explicit AstVisitor(AnalysisConfig config) : config_(std::move(config))
        {
        }

        /**
         * @brief Visits a Clang AST node and its children.
         * @param cursor The current AST node.
         * @param parent The parent AST node.
         * @return The result of the visit (continue, recurse, or break).
         */
        CXChildVisitResult visit(const CXCursor& cursor, const CXCursor& parent);

        /**
         * @brief Static callback for Clang's AST traversal.
         * @param cursor The current AST node.
         * @param parent The parent AST node.
         * @param client_data Pointer to the AstVisitor instance.
         * @return The result of the visit.
         */
        static CXChildVisitResult visitor_callback(CXCursor cursor, CXCursor parent, CXClientData client_data);

        /**
         * @brief Collects all base class cursors for a given class cursor.
         * @param cursor The class cursor.
         * @param bases Vector to store found base class cursors.
         */
        static void collect_all_base_cursors(const CXCursor& cursor, std::vector<CXCursor>& bases);

        /**
         * @brief Returns the analysis result after AST traversal.
         * @return The analysis result.
         */
        const AnalysisResult& result() const
        {
            return result_;
        }

    private:
        /**
         * @brief Analysis configuration.
         */
        AnalysisConfig config_;

        /**
         * @brief Result of the AST analysis.
         */
        AnalysisResult result_;

        /**
         * @brief Visits a class declaration node.
         * @param cursor The class declaration cursor.
         */
        void visit_class_decl(const CXCursor& cursor);

        /**
         * @brief Visits a class template declaration node.
         * @param cursor The class template cursor.
         */
        void visit_class_template(const CXCursor& cursor);

        /**
         * @brief Visits a function declaration node.
         * @param cursor The function declaration cursor.
         */
        void visit_function_decl(const CXCursor& cursor);

        /**
         * @brief Visits an enum declaration node.
         * @param cursor The enum declaration cursor.
         */
        void visit_enum_decl(const CXCursor& cursor);

        /**
         * @brief Parses an enum declaration cursor into an EnumDescriptor.
         * @param cursor The enum declaration cursor.
         * @return Optional EnumDescriptor if parsing is successful.
         */
        static std::optional<metadata::EnumDescriptor> parse_enum_decl(const CXCursor& cursor);

        /**
         * @brief Static callback for visiting class members.
         * @param cursor The member cursor.
         * @param parent The parent class cursor.
         * @param client_data Pointer to the class descriptor.
         * @return The result of the visit.
         */
        static CXChildVisitResult visit_class_member(CXCursor cursor, CXCursor parent, CXClientData client_data);

        /**
         * @brief Processes common logic for class declarations and templates.
         * @param cursor The class cursor.
         * @param class_desc The class descriptor to populate.
         */
        void process_class_common(const CXCursor& cursor, metadata::ClassDescriptor& class_desc);
    };
} // namespace codegen::analysis
