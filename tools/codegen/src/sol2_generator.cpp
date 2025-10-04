#include "sol2_generator.hpp"

namespace codegen {

    void Sol2Generator::generate(const std::vector<std::string>& includes,
                                 const std::vector<class_descriptor>& classes,
                                 const std::vector<function_descriptor>& free_functions)
    {
        std::ofstream out(m_output_file);
        if (!out.is_open()) {
            std::cerr << "Failed to open output file: " << m_output_file << std::endl;
            return;
        }

        write_line(out, 0, "// Auto-generated file. Do not edit manually!", 2);
        write_line(out, 0, "#include \"bindings/lua/member_registrar.hpp\"");
        write_line(out, 0, "#include \"bindings/lua/non_member_registrar.hpp\"");
        write_line(out, 0, "#include \"bindings/lua/registry.hpp\"", 2);

        for (const auto& include: includes) {
            write_line(out, 0, std::format("#include <{}>", include));
        }

        write_line(out, 0, "");
        write_line(out, 0, "#include <sol/sol.hpp>", 2);
        write_line(out, 0, "using namespace ogdf;", 2);

        write_line(out, 0, "namespace engine::bindings::lua");
        write_line(out, 0, "{");

        write_class_registrations(classes, out);

        write_line(out, 0, "");

        write_free_function_registrations(free_functions, out);

        write_line(out, 0, "}");
    }

    void Sol2Generator::write_class_registrations(const std::vector<class_descriptor>& classes, std::ofstream& out)
    {
        write_line(out, 1, "void register_members(sol::state& lua)");
        write_line(out, 1, "{");

        std::unordered_set<std::string> registered_bases;
        for (const auto& class_: classes) {
            for (const auto& base: class_.base_class_names()) {
                if (registered_bases.find(base) == registered_bases.end()) {
                    write_line(out, 2, std::format("MemberRegistrar<{}>(lua, \"{}\");", base, base));
                }
                registered_bases.insert(base);
            }
        }
        write_line(out, 0, "");

        for (const auto& class_: classes) {
            write_line(out, 2, std::format("MemberRegistrar<{}>(lua, \"{}\")", class_.name(), class_.name()));

            if (!class_.constructors().empty()) {
                for (const auto& ctor: class_.constructors()) {
                    write_line(out, 3, std::format(".constructor<{}>()", ctor.signature()));
                }
                write_line(out, 3, ".enable_garbage_collection()");
            }

            if (!class_.base_class_names().empty()) {
                for (const auto& base: class_.base_class_names()) {
                    write_line(out, 3, std::format(".bases<{}>()", base));
                }
            }

            if (!class_.member_variables().empty()) {
                for (const auto& var: class_.member_variables()) {
                    if (var.is_static()) {
                        write_line(out, 3, std::format(".variable(\"{}\", {}::{})", var.name(), class_.name(), var.name()));
                    }
                    else {
                        write_line(out, 3, std::format(".variable(\"{}\", &{}::{})", var.name(), class_.name(), var.name()));
                    }
                }
            }

            if (!class_.member_functions().empty()) {
                for (const auto& method: class_.member_functions()) {
                    if (method.is_const()) {
                        write_line(out, 3, std::format(".function(\"{}\", sol::resolve<{}{} const>(&{}::{}))",
                                   method.name(), method.return_type_name(),
                                   method.signature(), class_.name(),
                                   method.name()));
                    }
                    else {
                        write_line(out, 3, std::format(".function(\"{}\", sol::resolve<{}{}>(&{}::{}))",
                                   method.name(),
                                   method.return_type_name(),
                                   method.signature(),
                                   class_.name(), method.name()));
                    }
                }
            }
            write_line(out, 2, ";");
        }
        write_line(out, 1, "}");
    }

    void Sol2Generator::write_free_function_registrations(const std::vector<function_descriptor>& free_functions, std::ofstream& out)
    {
        write_line(out, 1, "void register_non_members(sol::state& lua)");
        write_line(out, 1, "{");

        if (!free_functions.empty()) {
            write_line(out, 2, "NonMemberRegistrar registrar(lua);");
            for (const auto& func: free_functions) {
                auto line = std::format("registrar.function(\"{}\", sol::resolve<{}{}>(&{}));",
                                        func.name(), func.return_type_name(), func.signature(), func.name());
                write_line(out, 2, line);
            }
        }
        write_line(out, 1, "}");
    }

    void Sol2Generator::write_line(std::ofstream& out, int indent_level, const std::string& text, int newlines)
    {
        out << std::string(indent_level * 4, ' ') << text;
        out << std::string(std::max(0, newlines), '\n');
    }

} // namespace codegen
