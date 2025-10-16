#include "analysis/ast_visitor.hpp"

#include "analysis/utils/clang_utils.hpp"

namespace codegen::analysis
{
    CXChildVisitResult AstVisitor::visit(CXCursor cursor, CXCursor parent)
    {
        const auto path = utils::get_include_path(cursor);
        if (path.empty() || !path.starts_with(config_.target_include_path))
        {
            return CXChildVisit_Continue;
        }

        const auto kind = clang_getCursorKind(cursor);

        if (kind == CXCursor_ClassDecl && clang_isCursorDefinition(cursor))
        {
            visit_class_decl(cursor);
        }
        else if (kind == CXCursor_EnumDecl && clang_isCursorDefinition(cursor))
        {
            visit_enum_decl(cursor);
        }
        else if (kind == CXCursor_FunctionDecl)
        {
            visit_function_decl(cursor);
        }

        return CXChildVisit_Recurse;
    }

    CXChildVisitResult AstVisitor::visitor_callback(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto* visitor = static_cast<AstVisitor*>(client_data);
        return visitor->visit(cursor, parent);
    }

    void AstVisitor::visit_class_decl(CXCursor cursor)
    {
        auto class_name = utils::get_spelling(cursor);
        if (!config_.target_classes.contains(class_name))
        {
            return;
        }

        result_.classes.emplace_back(std::move(class_name));
        result_.includes.insert(utils::get_include_path(cursor));

        clang_visitChildren(cursor, &AstVisitor::visit_class_member, this);
    }

    CXChildVisitResult AstVisitor::visit_class_member(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto* visitor = static_cast<AstVisitor*>(client_data);
        auto& class_desc = visitor->result_.classes.back();
        const auto child_kind = clang_getCursorKind(cursor);
        const auto parent_class_name = class_desc.name();

        switch (child_kind)
        {
        case CXCursor_Constructor:
            {
                metadata::ConstructorDescriptor ctor_desc(parent_class_name);
                for (const auto& param : utils::get_parameters(cursor))
                {
                    ctor_desc.add_parameter(param);
                }
                ctor_desc.set_signature(utils::build_signature(ctor_desc.parameters()));
                class_desc.add_constructor(std::move(ctor_desc));
                break;
            }

        case CXCursor_CXXBaseSpecifier:
            {
                CXCursor base_class = clang_getCursorReferenced(cursor);
                class_desc.add_base_class_name(utils::get_spelling(base_class));
                break;
            }

        case CXCursor_CXXMethod:
            {
                auto method_name = utils::get_spelling(cursor);
                const auto& target_methods = visitor->config_.target_classes.at(parent_class_name);
                if (std::ranges::find(target_methods, method_name) != target_methods.end())
                {
                    metadata::FunctionDescriptor method_desc(metadata::Scope::Member, method_name);
                    method_desc.set_return_type_name(utils::get_cursor_result_type_spelling(cursor));
                    method_desc.set_static(clang_CXXMethod_isStatic(cursor));
                    method_desc.set_const(clang_CXXMethod_isConst(cursor));
                    for (const auto& param : utils::get_parameters(cursor))
                    {
                        method_desc.add_parameter(param);
                    }
                    method_desc.set_signature(utils::build_signature(method_desc.parameters()));
                    class_desc.add_member_function(std::move(method_desc));
                }
                break;
            }

        case CXCursor_FieldDecl:
        case CXCursor_VarDecl:
            {
                if (clang_getCXXAccessSpecifier(cursor) == CX_CXXPublic)
                {
                    metadata::VariableDescriptor var_desc(metadata::Scope::Member, utils::get_spelling(cursor));
                    var_desc.set_type_name(utils::get_cursor_type_spelling(cursor));
                    var_desc.set_const(clang_isConstQualifiedType(clang_getCursorType(cursor)));
                    if (clang_Cursor_getStorageClass(cursor) == CX_SC_Static)
                    {
                        var_desc.set_static(true);
                    }
                    class_desc.add_member_variable(std::move(var_desc));
                }
                break;
            }

        default:
            break;
        }

        return CXChildVisit_Continue;
    }

    void AstVisitor::visit_enum_decl(CXCursor cursor)
    {
        metadata::EnumDescriptor enum_desc(utils::get_spelling(cursor));

        clang_visitChildren(cursor, [](CXCursor child, CXCursor, CXClientData data)-> CXChildVisitResult
        {
            if (clang_getCursorKind(child) == CXCursor_EnumConstantDecl)
            {
                auto* current_enum_desc = static_cast<metadata::EnumDescriptor*>(data);
                metadata::EnumeratorDescriptor enumerator;
                enumerator.name = utils::get_spelling(child);
                enumerator.value = clang_getEnumConstantDeclValue(child);
                current_enum_desc->add_enumerator(std::move(enumerator));
            }

            return CXChildVisit_Continue;
        }, &enum_desc);

        if (!enum_desc.enumerators().empty())
        {
            result_.enums.emplace_back(std::move(enum_desc));
        }
    }

    void AstVisitor::visit_function_decl(CXCursor cursor)
    {
        const auto func_name = utils::get_spelling(cursor);
        if (std::ranges::find(config_.target_free_functions, func_name) != config_.target_free_functions.end())
        {
            metadata::FunctionDescriptor func_desc(metadata::Scope::Global, func_name);
            func_desc.set_return_type_name(utils::get_cursor_result_type_spelling(cursor));
            for (const auto& param : utils::get_parameters(cursor))
            {
                func_desc.add_parameter(param);
            }
            func_desc.set_signature(utils::build_signature(func_desc.parameters()));
            result_.free_functions.emplace_back(std::move(func_desc));
            result_.includes.insert(utils::get_include_path(cursor));
        }
    }
} // namespace codegen::analysis
