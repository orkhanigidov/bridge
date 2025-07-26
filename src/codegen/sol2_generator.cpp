#include "codegen/sol2_generator.hpp"

namespace codegen
{
    void Sol2Generator::generate(const std::vector<std::string>& includes, const std::vector<class_descriptor>& classes,
                                 const std::vector<function_descriptor>& free_functions)
    {
        std::ofstream out(m_output_file);
        if (!out.is_open())
        {
            std::cerr << "Failed to open output file: " << m_output_file << std::endl;
            return;
        }

        write_line(out, 0, "// Auto-generated file. Do not edit manually!", 2);
        write_line(out, 0, "#include \"engine/bindings/lua/class_registrar.hpp\"");
        write_line(out, 0, "#include \"engine/bindings/lua/function_registrar.hpp\"");
        write_line(out, 0, "#include \"engine/bindings/lua/registrar.hpp\"", 2);

        for (const auto& include : includes)
        {
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

    void Sol2Generator::write_class_registrations(const std::vector<class_descriptor>& classes,
                                                  std::ofstream& out)
    {
        write_line(out, 1, "void Registrar::register_classes(sol::state& lua)");
        write_line(out, 1, "{");

        std::unordered_set<std::string> registered_bases;
        for (const auto& class_ : classes)
        {
            for (const auto& base : class_.base_class_names())
            {
                if (registered_bases.find(base) == registered_bases.end())
                {
                    write_line(out, 2, std::format("ClassRegistrar<{}>(lua, \"{}\");", base, base));
                }
                registered_bases.insert(base);
            }
        }
        write_line(out, 0, "");

        for (const auto& class_ : classes)
        {
            write_line(out, 2, std::format("ClassRegistrar<{}>(lua, \"{}\")", class_.name(), class_.name()));

            if (!class_.constructors().empty())
            {
                for (const auto& ctor : class_.constructors())
                {
                    write_line(out, 3, std::format(".constructor<{}>()", ctor.signature()));
                }
                write_line(out, 3, ".enable_gc()");
            }

            if (!class_.base_class_names().empty())
            {
                for (const auto& base : class_.base_class_names())
                {
                    write_line(out, 3, std::format(".base_classes<{}>()", base));
                }
            }

            if (!class_.variables().empty())
            {
                for (const auto& var : class_.variables())
                {
                    if (!var.is_static())
                    {
                        write_line(out, 3, std::format(".variable(\"{}\", &{}::{})", var.name(), class_.name(),
                                                       var.name()));
                    }
                    else
                    {
                        write_line(out, 3, std::format(".variable(\"{}\", {}::{})", var.name(), class_.name(),
                                                       var.name()));
                    }
                }
            }

            if (!class_.methods().empty())
            {
                for (const auto& method : class_.methods())
                {
                    if (method.is_const())
                    {
                        write_line(out, 3, std::format(".method(\"{}\", sol::resolve<{}{} const>(&{}::{}))",
                                                       method.name(), method.return_type(), method.signature(),
                                                       class_.name(), method.name()));
                    }
                    else
                    {
                        write_line(out, 3, std::format(".method(\"{}\", sol::resolve<{}{}>(&{}::{}))",
                                                       method.name(), method.return_type(), method.signature(),
                                                       class_.name(), method.name()));
                    }
                }
            }
            write_line(out, 2, ";");
        }
        write_line(out, 1, "}");
    }

    void Sol2Generator::write_free_function_registrations(const std::vector<function_descriptor>& free_functions,
                                                            std::ofstream& out)
    {
        write_line(out, 1, "void Registrar::register_free_functions(sol::state& lua)");
        write_line(out, 1, "{");

        if (!free_functions.empty())
        {
            write_line(out, 2, "FreeFunctionRegistrar registrar(lua);");
            for (const auto& func : free_functions)
            {
                auto line = std::format("registrar.function(\"{}\", sol::resolve<{}{}>(&{}));",
                                        func.name(), func.return_type(), func.signature(), func.name());
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
