/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file ast_visitor.cpp
 * @brief Implements the AST visitor for code analysis.
 */

#include "analysis/ast_visitor.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <clang-c/Index.h>

#include "analysis/utils/clang_utils.hpp"
#include "metadata/class_descriptor.hpp"
#include "metadata/constructor_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"
#include "metadata/function_descriptor.hpp"
#include "metadata/scope.hpp"
#include "metadata/variable_descriptor.hpp"

namespace
{
    /**
     * @brief Comparator for CXCursor objects based on their hash values.
     */
    struct CursorComparator
    {
        bool operator()(const CXCursor& lhs, const CXCursor& rhs) const
        {
            return clang_hashCursor(lhs) < clang_hashCursor(rhs);
        }
    };

    /**
     * @brief Retrieves the namespace of the given AST cursor, if any.
     * @param cursor The AST cursor whose namespace is to be determined.
     * @return An optional string containing the fully qualified namespace, or std::nullopt if none.
     */
    std::optional<std::string> get_cursor_namespace(const CXCursor& cursor)
    {
        std::string ns;
        CXCursor parent = clang_getCursorSemanticParent(cursor);

        while (true)
        {
            if (const CXCursorKind parent_kind = clang_getCursorKind(parent);
                parent_kind == CXCursor_Namespace)
            {
                std::string parent_name = codegen::analysis::utils::get_spelling(parent);
                ns = ns.empty() ? parent_name : parent_name + "::" + ns;
                parent = clang_getCursorSemanticParent(parent);
            }
            else if (parent_kind == CXCursor_TranslationUnit || parent_kind == CXCursor_InvalidFile)
            {
                break;
            }
            else
            {
                parent = clang_getCursorSemanticParent(parent);
            }
        }

        if (ns.empty())
        {
            return std::nullopt;
        }
        return ns;
    }

    /**
     * @brief Ensures a class has at least a default constructor.
     * @param cursor The class declaration cursor.
     * @param class_desc The class descriptor to update.
     */
    void check_and_add_default_constructor(const CXCursor& cursor, codegen::metadata::ClassDescriptor& class_desc)
    {
        if (!class_desc.constructors().empty())
        {
            return;
        }

        bool has_user_declared_ctor = false;
        clang_visitChildren(cursor, [](CXCursor child, CXCursor, const CXClientData client_data)
        {
            if (clang_getCursorKind(child) == CXCursor_Constructor)
            {
                *static_cast<bool*>(client_data) = true;
                return CXChildVisit_Break;
            }

            if (clang_getCursorKind(child) == CXCursor_CXXBaseSpecifier)
            {
                return CXChildVisit_Continue;
            }
            return CXChildVisit_Recurse;
        }, &has_user_declared_ctor);

        if (!has_user_declared_ctor)
        {
            codegen::metadata::ConstructorDescriptor ctor_desc(class_desc.name());
            ctor_desc.set_signature("()");
            class_desc.add_constructor(std::move(ctor_desc));
        }
    }

    bool is_iterable_recursive_check(const CXCursor& cursor, std::set<CXCursor, CursorComparator>& visited)
    {
        const CXType cursor_type = clang_getCursorType(cursor);
        const CXType canonical_type = clang_getCanonicalType(cursor_type);
        CXCursor definition_cursor = clang_getTypeDeclaration(canonical_type);
        if (clang_isInvalid(clang_getCursorKind(definition_cursor)) || clang_equalCursors(definition_cursor, clang_getNullCursor()))
        {
            definition_cursor = cursor;
        }

        CXCursor cursor_to_check = definition_cursor;
        if (const CXCursor template_cursor = clang_getSpecializedCursorTemplate(cursor_to_check);
            !clang_equalCursors(template_cursor, clang_getNullCursor()) && !clang_equalCursors(template_cursor, cursor_to_check))
        {
            cursor_to_check = template_cursor;
        }

        if (!visited.emplace(clang_getCanonicalCursor(cursor_to_check)).second)
        {
            return false;
        }

        bool has_begin = false;
        bool has_end = false;
        struct MethodCheckData
        {
            bool* has_begin;
            bool* has_end;
        };
        MethodCheckData data{&has_begin, &has_end};

        clang_visitChildren(cursor_to_check, [](CXCursor child, CXCursor, const CXClientData client_data)
        {
            if (clang_getCursorKind(child) == CXCursor_CXXMethod)
            {
                if (clang_getCXXAccessSpecifier(child) == CX_CXXPublic)
                {
                    const std::string method_name = codegen::analysis::utils::get_spelling(child);
                    const auto* check_data = static_cast<MethodCheckData*>(client_data);

                    if (method_name == "begin")
                    {
                        *check_data->has_begin = true;
                    }

                    if (method_name == "end")
                    {
                        *check_data->has_end = true;
                    }

                    if (*check_data->has_begin && *check_data->has_end)
                    {
                        return CXChildVisit_Break;
                    }
                }
            }
            return CXChildVisit_Recurse;
        }, &data);

        if (has_begin && has_end)
        {
            return true;
        }

        bool found_in_base = false;
        struct BaseVisitorData
        {
            std::set<CXCursor, CursorComparator>* visited;
            bool* found_in_base;
        };
        BaseVisitorData base_data{&visited, &found_in_base};

        clang_visitChildren(cursor_to_check, [](CXCursor child, CXCursor, const CXClientData client_data)
        {
            const auto* v_data = static_cast<BaseVisitorData*>(client_data);
            if (*v_data->found_in_base)
            {
                return CXChildVisit_Break;
            }

            std::cout << "Checking base: " << codegen::analysis::utils::get_spelling(child) << std::endl;
            if (clang_getCursorKind(child) == CXCursor_CXXBaseSpecifier)
            {
                if (clang_getCXXAccessSpecifier(child) == CX_CXXPublic || clang_getCXXAccessSpecifier(child) == CX_CXXPrivate)
                {
                    if (const CXCursor base_cursor = clang_getCursorReferenced(child);
                        is_iterable_recursive_check(base_cursor, *v_data->visited))
                    {
                        *v_data->found_in_base = true;
                        return CXChildVisit_Break;
                    }
                }
            }
            return CXChildVisit_Recurse;
        }, &base_data);

        return found_in_base;
    }

    /**
     * @brief Checks if the given type cursor represents an iterable container (has public begin and end methods).
     * @param cursor The type declaration cursor to check.
     * @param visitor Pointer to the AstVisitor instance for base class traversal.
     * @return True if the type is an iterable container, false otherwise.
     */
    bool is_iterable_container(const CXCursor& cursor, const codegen::analysis::AstVisitor* visitor)
    {
        std::set<CXCursor, CursorComparator> visited;
        return is_iterable_recursive_check(cursor, visited);
    }
}

namespace codegen::analysis
{
    /**
     * @brief Visits a Clang AST node and its children.
     * @param cursor The current AST node.
     * @param parent The parent AST node.
     * @return The result of the visit (continue, recurse, or break).
     */
    CXChildVisitResult AstVisitor::visit(const CXCursor& cursor, const CXCursor&)
    {
        if (const auto path = utils::get_include_path(cursor);
            path.empty() || !path.starts_with(config_.target_include_path.string()) && !path.starts_with(config_.wrapper_include_path.string()))
        {
            return CXChildVisit_Continue;
        }

        if (const auto kind = clang_getCursorKind(cursor);
            kind == CXCursor_ClassDecl && clang_isCursorDefinition(cursor))
        {
            visit_class_decl(cursor);
        }
        else if (kind == CXCursor_ClassTemplate)
        {
            visit_class_template(cursor);
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

    /**
     * @brief Static callback for Clang's AST traversal.
     * @param cursor The current AST node.
     * @param parent The parent AST node.
     * @param client_data Pointer to the AstVisitor instance.
     * @return The result of the visit.
     */
    CXChildVisitResult AstVisitor::visitor_callback(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto* visitor = static_cast<AstVisitor*>(client_data);
        return visitor->visit(cursor, parent);
    }

    /**
     * @brief Collects all base class cursors for a given class cursor.
     * @param cursor The class cursor.
     * @param bases Vector to store found base class cursors.
     */
    void AstVisitor::collect_all_base_cursors(const CXCursor& cursor, std::vector<CXCursor>& bases)
    {
        struct VisitorData
        {
            std::set<CXCursor, CursorComparator>* visited;
            std::vector<CXCursor>* stack;
            std::vector<CXCursor>* bases;
        };

        std::set<CXCursor, CursorComparator> visited;
        std::vector<CXCursor> stack;

        visited.emplace(cursor);
        stack.emplace_back(cursor);

        VisitorData data{&visited, &stack, &bases};

        while (!stack.empty())
        {
            const CXCursor current = stack.back();
            stack.pop_back();

            clang_visitChildren(current, [](CXCursor child, CXCursor, const CXClientData client_data)
            {
                const auto* visitor = static_cast<VisitorData*>(client_data);
                if (clang_getCursorKind(child) == CXCursor_CXXBaseSpecifier)
                {
                    if (clang_getCXXAccessSpecifier(child) == CX_CXXPublic)
                    {
                        if (CXCursor base = clang_getCursorReferenced(child);
                            visitor->visited->emplace(base).second)
                        {
                            visitor->bases->emplace_back(base);
                            visitor->stack->emplace_back(base);
                        }
                    }
                }
                return CXChildVisit_Continue;
            }, &data);
        }
    }

    /**
     * @brief Parses an enum declaration cursor into an EnumDescriptor.
     * @param cursor The enum declaration cursor.
     * @return Optional EnumDescriptor if parsing is successful.
     */
    std::optional<metadata::EnumDescriptor> AstVisitor::parse_enum_decl(const CXCursor& cursor)
    {
        const auto enum_name = utils::get_spelling(cursor);
        if (enum_name.empty() || enum_name.starts_with("(unnamed"))
        {
            return std::nullopt;
        }

        metadata::EnumDescriptor enum_desc(enum_name);

        clang_visitChildren(cursor, [](CXCursor child, CXCursor, const CXClientData client_data)
        {
            if (clang_getCursorKind(child) == CXCursor_EnumConstantDecl)
            {
                auto* current_enum_desc = static_cast<metadata::EnumDescriptor*>(client_data);
                current_enum_desc->add_enumerator({
                    .name = utils::get_spelling(child),
                    .value = clang_getEnumConstantDeclValue(child)
                });
            }
            return CXChildVisit_Continue;
        }, &enum_desc);

        if (enum_desc.enumerators().empty())
        {
            return std::nullopt;
        }

        return enum_desc;
    }

    /**
     * @brief Processes common logic for class declarations and templates.
     * @param cursor The class cursor.
     * @param class_desc The class descriptor to populate.
     */
    void AstVisitor::process_class_common(const CXCursor& cursor, metadata::ClassDescriptor& class_desc)
    {
        clang_visitChildren(cursor, &AstVisitor::visit_class_member, this);

        std::vector<CXCursor> all_bases;
        collect_all_base_cursors(cursor, all_bases);
        for (const auto base : all_bases)
        {
            clang_visitChildren(base, &AstVisitor::visit_class_member, this);
        }

        check_and_add_default_constructor(cursor, class_desc);
    }

    /**
     * @brief Visits a class declaration node.
     * @param cursor The class declaration cursor.
     */
    void AstVisitor::visit_class_decl(const CXCursor& cursor)
    {
        auto class_name = utils::get_spelling(cursor);
        if (!config_.target_classes.contains(class_name))
        {
            return;
        }

        const auto& class_config = config_.target_classes.at(class_name);
        if (!class_config.types.empty())
        {
            return;
        }

        if (result_.processed_classes.contains(class_name))
        {
            return;
        }
        result_.processed_classes.emplace(class_name);

        if (const auto ns = get_cursor_namespace(cursor))
        {
            result_.namespaces.emplace(*ns);
        }

        result_.includes.emplace(utils::get_include_path(cursor));
        result_.classes.emplace_back(std::move(class_name));

        process_class_common(cursor, result_.classes.back());
    }

    /**
     * @brief Static callback for visiting class members.
     * @param cursor The member cursor.
     * @param parent The parent class cursor.
     * @param client_data Pointer to the class descriptor.
     * @return The result of the visit.
     */
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
                std::string parent_name = utils::get_spelling(parent);
                std::string class_lookup_name = parent_class_name;
                if (size_t template_bracket_pos = class_lookup_name.find('<'); template_bracket_pos != std::string::npos)
                {
                    class_lookup_name = class_lookup_name.substr(0, template_bracket_pos);
                }

                if (parent_name != class_lookup_name)
                {
                    break;
                }

                // TODO: Handle deleted constructors properly

                if (clang_getCXXAccessSpecifier(cursor) != CX_CXXPublic)
                {
                    break;
                }

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
                if (clang_getCXXAccessSpecifier(cursor) != CX_CXXPublic)
                {
                    break;
                }

                const std::string& derived_class_name = class_desc.name();
                std::string derived_template_args;

                if (size_t template_bracket_pos = derived_class_name.find('<'); template_bracket_pos != std::string::npos)
                {
                    derived_template_args = derived_class_name.substr(template_bracket_pos);
                }

                std::string base_class_name = utils::get_cursor_type_spelling(cursor);

                if (!derived_template_args.empty())
                {
                    if (size_t base_bracket_pos = base_class_name.find('<'); base_bracket_pos != std::string::npos)
                    {
                        base_class_name = base_class_name.substr(0, base_bracket_pos) + derived_template_args;
                    }
                }

                class_desc.add_base_class_name(std::move(base_class_name));

                CXCursor base_class_decl = clang_getCursorReferenced(cursor);
                if (auto ns = get_cursor_namespace(base_class_decl))
                {
                    visitor->result_.namespaces.emplace(std::move(*ns));
                }
                break;
            }

        case CXCursor_CXXMethod:
            {
                auto method_name = utils::get_spelling(cursor);
                std::string class_lookup_name = parent_class_name;
                if (size_t template_bracket_pos = class_lookup_name.find('<'); template_bracket_pos != std::string::npos)
                {
                    class_lookup_name = class_lookup_name.substr(0, template_bracket_pos);
                }

                if (visitor->config_.target_classes.contains(class_lookup_name))
                {
                    const auto& class_config = visitor->config_.target_classes.at(class_lookup_name);
                    if (const auto& target_methods = class_config.methods;
                        std::ranges::find(target_methods, method_name) != target_methods.end())
                    {
                        CXType return_type = clang_getCursorResultType(cursor);

                        if (return_type.kind == CXType_LValueReference || return_type.kind == CXType_RValueReference)
                        {
                            return_type = clang_getPointeeType(return_type);
                        }
                        if (return_type.kind == CXType_Pointer)
                        {
                            return_type = clang_getPointeeType(return_type);
                        }

                        return_type = clang_getUnqualifiedType(return_type);

                        if (const CXCursor type_decl = clang_getTypeDeclaration(return_type);
                            is_iterable_container(type_decl, visitor))
                        {
                            std::string container_type_name = utils::get_type_spelling(return_type);
                            visitor->result_.containers.emplace(std::move(container_type_name));
                        }

                        metadata::FunctionDescriptor method_desc(metadata::Scope::Member, method_name, utils::get_cursor_result_type_spelling(cursor));
                        method_desc.set_static(clang_CXXMethod_isStatic(cursor));
                        method_desc.set_const(clang_CXXMethod_isConst(cursor));
                        for (const auto& param : utils::get_parameters(cursor))
                        {
                            method_desc.add_parameter(param);
                        }
                        method_desc.set_signature(utils::build_signature(method_desc.parameters()));
                        class_desc.add_member_function(std::move(method_desc));
                    }
                }
                break;
            }

        case CXCursor_EnumDecl:
            {
                if (clang_isCursorDefinition(cursor))
                {
                    if (auto enum_desc_opt = visitor->parse_enum_decl(cursor))
                    {
                        class_desc.add_member_enumerator(std::move(*enum_desc_opt));
                        visitor->result_.includes.emplace(utils::get_include_path(cursor));
                    }
                }
                break;
            }

        case CXCursor_FieldDecl:
        case CXCursor_VarDecl:
            {
                if (clang_getCXXAccessSpecifier(cursor) == CX_CXXPublic)
                {
                    metadata::VariableDescriptor var_desc(metadata::Scope::Member, utils::get_spelling(cursor), utils::get_cursor_type_spelling(cursor));
                    var_desc.set_const(clang_isConstQualifiedType(clang_getCursorType(cursor)));
                    var_desc.set_static(clang_Cursor_getStorageClass(cursor) == CX_SC_Static);

                    CXType var_type = clang_getCursorType(cursor);

                    if (var_type.kind == CXType_LValueReference || var_type.kind == CXType_RValueReference)
                    {
                        var_type = clang_getPointeeType(var_type);
                    }
                    if (var_type.kind == CXType_Pointer)
                    {
                        var_type = clang_getPointeeType(var_type);
                    }

                    var_type = clang_getUnqualifiedType(var_type);

                    CXCursor type_decl = clang_getTypeDeclaration(var_type);
                    bool is_container_type = is_iterable_container(type_decl, visitor);
                    var_desc.set_container(is_container_type);
                    class_desc.add_member_variable(std::move(var_desc));
                }
                break;
            }

        default:
            break;
        }

        return CXChildVisit_Continue;
    }

    /**
     * @brief Visits a class template declaration node.
     * @param cursor The class template cursor.
     */
    void AstVisitor::visit_class_template(const CXCursor& cursor)
    {
        auto template_name = utils::get_spelling(cursor);
        if (!config_.target_classes.contains(template_name))
        {
            return;
        }

        const auto& template_config = config_.target_classes.at(template_name);
        for (const auto& type : template_config.types)
        {
            std::string full_name = std::format("{}<{}>", template_name, type);

            if (result_.processed_classes.contains(full_name))
            {
                continue;
            }
            result_.processed_classes.emplace(full_name);

            if (auto ns = get_cursor_namespace(cursor))
            {
                result_.namespaces.emplace(std::move(*ns));
            }

            result_.includes.emplace(utils::get_include_path(cursor));
            result_.classes.emplace_back(std::move(full_name));
            result_.classes.back().add_template_type(type);

            process_class_common(cursor, result_.classes.back());
        }
    }

    /**
     * @brief Visits an enum declaration node.
     * @param cursor The enum declaration cursor.
     */
    void AstVisitor::visit_enum_decl(const CXCursor& cursor)
    {
        const auto enum_name = utils::get_spelling(cursor);
        if (const auto& target_enums = config_.target_enums;
            std::ranges::find(target_enums, enum_name) != target_enums.end())
        {
            if (auto enum_desc_opt = parse_enum_decl(cursor))
            {
                result_.enums.emplace_back(std::move(*enum_desc_opt));
                result_.includes.emplace(utils::get_include_path(cursor));
            }
        }
    }

    /**
     * @brief Visits a function declaration node.
     * @param cursor The function declaration cursor.
     */
    void AstVisitor::visit_function_decl(const CXCursor& cursor)
    {
        const auto path = utils::get_include_path(cursor);
        const bool is_wrapper = !path.empty() && path.starts_with(config_.wrapper_include_path.string());

        const auto func_name = utils::get_spelling(cursor);

        if (is_wrapper)
        {
            for (const auto& [class_name_from_config, class_config] : config_.target_classes)
            {
                if (std::ranges::find(class_config.methods, func_name) != class_config.methods.end())
                {
                    auto it = std::ranges::find_if(result_.classes, [&](const auto& cls)
                    {
                        std::string base_class_name = cls.name();
                        if (size_t template_pos = base_class_name.find('<'); template_pos != std::string::npos)
                        {
                            base_class_name = base_class_name.substr(0, template_pos);
                        }
                        return base_class_name == class_name_from_config;
                    });

                    if (it != result_.classes.end())
                    {
                        metadata::FunctionDescriptor method_desc(metadata::Scope::Member, func_name, utils::get_cursor_result_type_spelling(cursor));

                        for (const auto& param : utils::get_parameters(cursor))
                        {
                            method_desc.add_parameter(param);
                        }
                        method_desc.set_signature(utils::build_signature(method_desc.parameters()));

                        method_desc.set_wrapper(true);

                        it->add_member_function(std::move(method_desc));
                        result_.includes.emplace(utils::get_include_path(cursor));

                        return;
                    }
                }
            }
        }

        if (const auto& target_funcs = config_.target_free_functions;
            is_wrapper || std::ranges::find(target_funcs, func_name) != target_funcs.end())
        {
            CXType return_type = clang_getCursorResultType(cursor);

            if (return_type.kind == CXType_LValueReference || return_type.kind == CXType_RValueReference)
            {
                return_type = clang_getPointeeType(return_type);
            }
            if (return_type.kind == CXType_Pointer)
            {
                return_type = clang_getPointeeType(return_type);
            }

            return_type = clang_getUnqualifiedType(return_type);

            if (const CXCursor type_decl = clang_getTypeDeclaration(return_type);
                is_iterable_container(type_decl, this))
            {
                std::string container_type_name = utils::get_type_spelling(return_type);
                result_.containers.emplace(std::move(container_type_name));
            }

            metadata::FunctionDescriptor func_desc(metadata::Scope::Global, func_name, utils::get_cursor_result_type_spelling(cursor));
            for (const auto& param : utils::get_parameters(cursor))
            {
                func_desc.add_parameter(param);
            }
            func_desc.set_signature(utils::build_signature(func_desc.parameters()));
            result_.free_functions.emplace_back(std::move(func_desc));
            result_.includes.emplace(utils::get_include_path(cursor));
        }
    }
} // namespace codegen::analysis
