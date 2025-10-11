#include "analysis/clang_analyzer.hpp"
#include "io/yaml_reader.hpp"

namespace codegen::analysis
{
    /**
     *
     * @param filename
     * @param target_classes
     * @param target_free_functions
     */
    ClangAnalyzer::ClangAnalyzer(const std::string& filename,
                                 const std::unordered_map<std::string, std::vector<std::string>>& target_classes,
                                 const std::vector<std::string>& target_free_functions)
        : target_classes_(target_classes), target_free_functions_(target_free_functions)
    {
        found_classes_.clear();
        found_free_functions_.clear();

        CXIndex index = clang_createIndex(0, 0);

        const std::vector<std::string> clang_args_str = {
            "-x", "c++", "-std=c++17", "-DOGDF_EXPORT=", "-IE:/THESIS/Engine.Cpp/build/_deps/ogdf-src/include"
        };
        std::vector<const char*> clang_args;
        for (const auto& arg : clang_args_str)
        {
            clang_args.emplace_back(arg.c_str());
        }

        CXTranslationUnit translation_unit = clang_parseTranslationUnit(index, filename.c_str(), clang_args.data(),
                                                                        static_cast<int>(clang_args.size()), nullptr, 0,
                                                                        CXTranslationUnit_None);
        if (!translation_unit)
        {
            std::cerr << "Failed to parse: " << filename << std::endl;
            clang_disposeIndex(index);
            return;
        }

        const CXCursor cursor = clang_getTranslationUnitCursor(translation_unit);
        clang_visitChildren(cursor, &ClangAnalyzer::visitor, this);

        clang_disposeTranslationUnit(translation_unit);
        clang_disposeIndex(index);
    }

    /**
     *
     * @param cursor
     * @param parent
     * @param client_data
     * @return
     */
    CXChildVisitResult
    ClangAnalyzer::visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto* analyzer = static_cast<ClangAnalyzer*>(client_data);
        if (!analyzer)
        {
            return CXChildVisit_Continue;
        }
        const auto kind = clang_getCursorKind(cursor);

        if (kind == CXCursor_ClassDecl && clang_isCursorDefinition(cursor))
        {
            auto class_name = get_spelling(cursor);

            if (analyzer->target_classes_.find(class_name) != analyzer->target_classes_.end())
            {
                engine::metadata::ClassDescriptor class_desc;
                class_desc.set_name(class_name);
                analyzer->found_classes_.emplace_back(class_desc);

                clang_visitChildren(cursor, [](CXCursor child_cursor, CXCursor parent_cursor,
                                               CXClientData client_data) -> CXChildVisitResult
                {
                    auto* analyzer = static_cast<ClangAnalyzer*>(client_data);
                    if (!analyzer)
                    {
                        return CXChildVisit_Continue;
                    }
                    const auto child_kind = clang_getCursorKind(child_cursor);

                    if (child_kind == CXCursor_Constructor)
                    {
                        auto ctor_name = get_spelling(child_cursor);
                        auto class_name = get_spelling(parent_cursor);

                        engine::metadata::ConstructorDescriptor ctor_desc;
                        ctor_desc.set_class_name(ctor_name);

                        std::string overload_signature;
                        auto params = get_arguments(child_cursor);
                        for (size_t i = 0; i < params.size(); ++i)
                        {
                            ctor_desc.add_parameter(params[i]);
                            overload_signature += params[i].type_name();
                            if (i != params.size() - 1)
                            {
                                overload_signature += ", ";
                            }
                        }
                        ctor_desc.set_signature(overload_signature);

                        analyzer->found_classes_.back().add_constructor(ctor_desc);
                    }
                    else if (child_kind == CXCursor_CXXBaseSpecifier)
                    {
                        CXCursor referenced = clang_getCursorReferenced(child_cursor);
                        auto base_class_name = get_spelling(referenced);
                        analyzer->found_classes_.back().add_base_class_name(base_class_name);
                    }
                    else if (child_kind == CXCursor_CXXMethod)
                    {
                        auto method_name = get_spelling(child_cursor);
                        auto class_name = get_spelling(parent_cursor);

                        const auto& target_methods = analyzer->target_classes_.at(class_name);

                        if (std::find(target_methods.begin(), target_methods.end(), method_name) != target_methods.
                            end())
                        {
                            engine::metadata::FunctionDescriptor method_desc;
                            method_desc.set_name(method_name);
                            method_desc.set_return_type_name(get_result_type(child_cursor));
                            if (clang_CXXMethod_isStatic(child_cursor))
                            {
                                method_desc.set_static(true);
                            }
                            if (clang_CXXMethod_isConst(child_cursor))
                            {
                                method_desc.set_const(true);
                            }

                            std::string overload_signature = "(";
                            auto params = get_arguments(child_cursor);
                            for (size_t i = 0; i < params.size(); ++i)
                            {
                                method_desc.add_parameter(params[i]);
                                overload_signature += params[i].type_name();
                                if (i != params.size() - 1)
                                {
                                    overload_signature += ", ";
                                }
                            }
                            overload_signature += ")";
                            method_desc.set_signature(overload_signature);

                            analyzer->found_classes_.back().add_member_function(method_desc);
                        }
                    }
                    else if (child_kind == CXCursor_FieldDecl || child_kind == CXCursor_VarDecl)
                    {
                        if (clang_getCXXAccessSpecifier(child_cursor) == CX_CXXPublic)
                        {
                            std::string var_name = get_spelling(child_cursor);
                            std::string var_type = get_type(child_cursor);

                            engine::metadata::VariableDescriptor var_desc;
                            var_desc.set_name(var_name);
                            var_desc.set_type_name(var_type);
                            if (clang_Cursor_getStorageClass(child_cursor) == CX_SC_Static)
                            {
                                var_desc.set_static(true);
                            }
                            analyzer->found_classes_.back().add_member_variable(var_desc);
                        }
                    }
                    return CXChildVisit_Continue;
                }, client_data);

                analyzer->found_includes_.emplace_back(get_include_path(cursor));
            }
        }
        else if (kind == CXCursor_FunctionDecl)
        {
            auto func_name = get_spelling(cursor);

            if (std::find(analyzer->target_free_functions_.begin(), analyzer->target_free_functions_.end(), func_name)
                != analyzer->target_free_functions_.end())
            {
                engine::metadata::FunctionDescriptor func_desc;
                func_desc.set_name(func_name);
                func_desc.set_return_type_name(get_result_type(cursor));

                std::string overload_signature = "(";
                auto params = get_arguments(cursor);
                for (size_t i = 0; i < params.size(); ++i)
                {
                    func_desc.add_parameter(params[i]);
                    overload_signature += params[i].type_name();
                    if (i != params.size() - 1)
                    {
                        overload_signature += ", ";
                    }
                }
                overload_signature += ")";
                func_desc.set_signature(overload_signature);

                analyzer->found_free_functions_.emplace_back(func_desc);
                analyzer->found_includes_.emplace_back(get_include_path(cursor));
            }
        }
        return CXChildVisit_Recurse;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::string
    ClangAnalyzer::get_spelling(const CXCursor& cursor)
    {
        const CXString cursor_spelling = clang_getCursorSpelling(cursor);
        std::string result = clang_getCString(cursor_spelling);
        clang_disposeString(cursor_spelling);
        return result;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::string
    ClangAnalyzer::get_result_type(const CXCursor& cursor)
    {
        const CXType return_type = clang_getCursorResultType(cursor);
        const CXString type_spelling = clang_getTypeSpelling(return_type);
        std::string result = clang_getCString(type_spelling);
        clang_disposeString(type_spelling);
        return result;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::string
    ClangAnalyzer::get_type(const CXCursor& cursor)
    {
        const CXType type = clang_getCursorType(cursor);
        const CXString type_spelling = clang_getTypeSpelling(type);
        std::string result = clang_getCString(type_spelling);
        clang_disposeString(type_spelling);
        return result;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::string
    ClangAnalyzer::get_qualified_type(const CXCursor& cursor)
    {
        const CXType type = clang_getCursorType(cursor);
        const CXCursor type_decl = clang_getTypeDeclaration(type);
        std::string type_name = get_type(cursor);

        if (!clang_Cursor_isNull(type_decl))
        {
            CXCursor parent = clang_getCursorSemanticParent(type_decl);
            std::string parent_name = get_spelling(parent);
            if (!parent_name.empty())
            {
                return parent_name + "::" + type_name;
            }
        }
        return type_name;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::vector<engine::metadata::ParameterDescriptor>
    ClangAnalyzer::get_arguments(const CXCursor& cursor)
    {
        std::vector<engine::metadata::ParameterDescriptor> params;
        const int num_args = clang_Cursor_getNumArguments(cursor);
        for (int i = 0; i < num_args; ++i)
        {
            CXCursor param_cursor = clang_Cursor_getArgument(cursor, i);
            auto param_name = get_spelling(param_cursor);
            auto param_type = get_qualified_type(param_cursor);
            engine::metadata::ParameterDescriptor param_desc;
            param_desc.set_name(param_name);
            param_desc.set_type_name(param_type);
            params.push_back(param_desc);
        }
        return params;
    }

    /**
     *
     * @param cursor
     * @return
     */
    std::string
    ClangAnalyzer::get_include_path(const CXCursor& cursor)
    {
        const CXSourceLocation location = clang_getCursorLocation(cursor);
        CXString filename;
        unsigned line, column;
        clang_getPresumedLocation(location, &filename, &line, &column);
        std::string path = clang_getCString(filename);
        clang_disposeString(filename);
        std::ranges::replace(path, '\\', '/');
        return path;
    }
} // namespace codegen::analysis
