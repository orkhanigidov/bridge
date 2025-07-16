#include "codegen/clang_analyzer.hpp"

#include "codegen/yaml_reader.hpp"

namespace codegen
{
    ClangAnalyzer::ClangAnalyzer(const std::string& filename, const std::unordered_map<std::string, std::vector<std::string>>& target_classes, const std::vector<std::string>& target_global_functions)
        : target_classes_(target_classes), target_global_functions_(target_global_functions)
    {
        found_classes_.clear();
        found_global_functions_.clear();

        const CXIndex index = clang_createIndex(0, 0);

        const std::vector<std::string> clang_args_str = {"-x", "c++", "-std=c++17", "-DOGDF_EXPORT=", "-IE:/THESIS/Engine.Cpp/build/_deps/ogdf-src/include"};
        std::vector<const char*> clang_args;
        for (const auto& arg : clang_args_str)
        {
            clang_args.emplace_back(arg.c_str());
        }

        const CXTranslationUnit translation_unit = clang_parseTranslationUnit(index, filename.c_str(), clang_args.data(), static_cast<int>(clang_args.size()), nullptr, 0, CXTranslationUnit_None);
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

    CXChildVisitResult ClangAnalyzer::visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto* analyzer = static_cast<ClangAnalyzer*>(client_data);
        if (!analyzer)
        {
            return CXChildVisit_Continue;
        }
        const auto kind = clang_getCursorKind(cursor);

        if (kind == CXCursor_ClassDecl && clang_isCursorDefinition(cursor))
        {
            auto class_name = getSpelling(cursor);

            if (analyzer->target_classes_.find(class_name) != analyzer->target_classes_.end())
            {
                engine::meta::ClassDescriptor class_desc;
                class_desc.setName(class_name);
                analyzer->found_classes_.emplace_back(class_desc);

                clang_visitChildren(
                    cursor,
                    [](CXCursor child_cursor, CXCursor parent_cursor, CXClientData client_data) -> CXChildVisitResult
                    {
                        auto* analyzer = static_cast<ClangAnalyzer*>(client_data);
                        if (!analyzer)
                        {
                            return CXChildVisit_Continue;
                        }
                        const auto child_kind = clang_getCursorKind(child_cursor);

                        if (child_kind == CXCursor_Constructor)
                        {
                            auto ctor_name  = analyzer->getSpelling(child_cursor);
                            auto class_name = analyzer->getSpelling(parent_cursor);

                            engine::meta::FunctionDescriptor ctor_desc;
                            ctor_desc.setName(ctor_name);
                            ctor_desc.setReturnType("");

                            std::string overload_signature = "";
                            auto params                    = analyzer->getArguments(child_cursor);
                            for (size_t i = 0; i < params.size(); ++i)
                            {
                                ctor_desc.addParameter(params[i]);
                                overload_signature += params[i].type();
                                if (i != params.size() - 1)
                                {
                                    overload_signature += ", ";
                                }
                            }
                            ctor_desc.setSignature(overload_signature);

                            analyzer->found_classes_.back().addConstructor(ctor_desc);
                        }
                        else if (child_kind == CXCursor_CXXBaseSpecifier)
                        {
                            CXCursor referenced  = clang_getCursorReferenced(child_cursor);
                            auto base_class_name = analyzer->getSpelling(referenced);
                            analyzer->found_classes_.back().addBaseClass(base_class_name);
                        }
                        else if (child_kind == CXCursor_CXXMethod)
                        {
                            auto method_name = analyzer->getSpelling(child_cursor);
                            auto class_name  = analyzer->getSpelling(parent_cursor);

                            const auto& target_methods = analyzer->target_classes_.at(class_name);

                            if (std::find(target_methods.begin(), target_methods.end(), method_name) != target_methods.end())
                            {
                                engine::meta::FunctionDescriptor method_desc;
                                method_desc.setName(method_name);
                                method_desc.setReturnType(analyzer->getResultType(child_cursor));
                                if (clang_CXXMethod_isStatic(child_cursor))
                                {
                                    method_desc.setStatic(true);
                                }
                                if (clang_CXXMethod_isConst(child_cursor))
                                {
                                    method_desc.setConst(true);
                                }

                                std::string overload_signature = "(";
                                auto params                    = analyzer->getArguments(child_cursor);
                                for (size_t i = 0; i < params.size(); ++i)
                                {
                                    method_desc.addParameter(params[i]);
                                    overload_signature += params[i].type();
                                    if (i != params.size() - 1)
                                    {
                                        overload_signature += ", ";
                                    }
                                }
                                overload_signature += ")";
                                method_desc.setSignature(overload_signature);

                                analyzer->found_classes_.back().addMethod(method_desc);
                            }
                        }
                        else if (child_kind == CXCursor_FieldDecl || child_kind == CXCursor_VarDecl)
                        {
                            if (clang_getCXXAccessSpecifier(child_cursor) == CX_CXXPublic)
                            {
                                std::string var_name = analyzer->getSpelling(child_cursor);
                                std::string var_type = analyzer->getType(child_cursor);

                                engine::meta::VariableDescriptor var_desc;
                                var_desc.setName(var_name);
                                var_desc.setType(var_type);
                                if (clang_Cursor_getStorageClass(child_cursor) == CX_SC_Static)
                                {
                                    var_desc.setStatic(true);
                                }
                                analyzer->found_classes_.back().addVariable(var_desc);
                            }
                        }
                        return CXChildVisit_Continue;
                    },
                    client_data);

                analyzer->found_includes_.emplace_back(getIncludePath(cursor));
            }
        }
        else if (kind == CXCursor_FunctionDecl)
        {
            auto func_name = getSpelling(cursor);

            if (std::find(analyzer->target_global_functions_.begin(), analyzer->target_global_functions_.end(), func_name) != analyzer->target_global_functions_.end())
            {
                engine::meta::FunctionDescriptor func_desc;
                func_desc.setName(func_name);
                func_desc.setReturnType(getResultType(cursor));

                std::string overload_signature = "(";
                auto params                    = analyzer->getArguments(cursor);
                for (size_t i = 0; i < params.size(); ++i)
                {
                    func_desc.addParameter(params[i]);
                    overload_signature += params[i].type();
                    if (i != params.size() - 1)
                    {
                        overload_signature += ", ";
                    }
                }
                overload_signature += ")";
                func_desc.setSignature(overload_signature);

                analyzer->found_global_functions_.emplace_back(func_desc);
                analyzer->found_includes_.emplace_back(getIncludePath(cursor));
            }
        }
        return CXChildVisit_Recurse;
    }

    std::string ClangAnalyzer::getSpelling(const CXCursor& cursor)
    {
        const CXString cursor_spelling = clang_getCursorSpelling(cursor);
        std::string result             = clang_getCString(cursor_spelling);
        clang_disposeString(cursor_spelling);
        return result;
    }

    std::string ClangAnalyzer::getResultType(const CXCursor& cursor)
    {
        const CXType return_type     = clang_getCursorResultType(cursor);
        const CXString type_spelling = clang_getTypeSpelling(return_type);
        std::string result           = clang_getCString(type_spelling);
        clang_disposeString(type_spelling);
        return result;
    }

    std::string ClangAnalyzer::getType(const CXCursor& cursor)
    {
        const CXType type            = clang_getCursorType(cursor);
        const CXString type_spelling = clang_getTypeSpelling(type);
        std::string result           = clang_getCString(type_spelling);
        clang_disposeString(type_spelling);
        return result;
    }

    std::vector<engine::meta::ParameterDescriptor> ClangAnalyzer::getArguments(const CXCursor& cursor)
    {
        std::vector<engine::meta::ParameterDescriptor> params;
        const int num_args = clang_Cursor_getNumArguments(cursor);
        for (int i = 0; i < num_args; ++i)
        {
            CXCursor param_cursor = clang_Cursor_getArgument(cursor, i);
            auto param_name       = getSpelling(param_cursor);
            auto param_type       = getType(param_cursor);
            engine::meta::ParameterDescriptor param_desc;
            param_desc.setName(param_name);
            param_desc.setType(param_type);
            params.push_back(param_desc);
        }
        return params;
    }

    std::string ClangAnalyzer::getIncludePath(const CXCursor& cursor)
    {
        CXSourceLocation location = clang_getCursorLocation(cursor);
        CXString filename;
        unsigned line, column;
        clang_getPresumedLocation(location, &filename, &line, &column);
        std::string path = clang_getCString(filename);
        clang_disposeString(filename);
        std::replace(path.begin(), path.end(), '\\', '/');
        return path;
    }
} // namespace codegen