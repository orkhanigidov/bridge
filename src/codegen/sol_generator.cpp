#include "codegen/sol_generator.hpp"

namespace codegen
{
    SolGenerator::SolGenerator(const std::string& output_filename) : output_filename_(output_filename) {}

    void SolGenerator::generate(const std::vector<engine::meta::ClassDescriptor>& classes, const std::vector<engine::meta::FunctionDescriptor>& global_functions,
                                const std::vector<std::string>& includes)
    {
        std::ofstream out(output_filename_);
        if (!out.is_open())
        {
            std::cerr << "Failed to open output file: " << output_filename_ << std::endl;
            return;
        }

        out << "// Auto-generated file. Do not edit manually!\n\n";
        out << "#include \"engine/lua_bindings/class_registrar.hpp\"\n";
        out << "#include \"engine/lua_bindings/function_registrar.hpp\"\n\n";

        for (const auto& include : includes)
        {
            out << "#include <" << include << ">\n";
        }

        out << "\n";
        out << "#include <sol/sol.hpp>\n\n";

        out << "using namespace ogdf;\n\n";

        out << "namespace engine::lua_bindings\n";
        out << "{\n";

        generate_classes(classes, out);

        out << "\n";

        generate_global_functions(global_functions, out);

        out << "}\n";
    }

    void SolGenerator::generate_classes(const std::vector<engine::meta::ClassDescriptor>& classes, std::ofstream& out)
    {
        out << "    void register_classes(sol::state& lua)\n";
        out << "    {\n";

        for (const auto& class_ : classes)
        {
            for (const auto& base : class_.baseClasses())
            {
                out << "    class_registrar<" << base << ">(lua, \"" << base << "\");\n\n";
            }
        }

        for (const auto& class_ : classes)
        {
            out << "    class_registrar<" << class_.name() << ">(lua, \"" << class_.name() << "\")\n";

            if (!class_.constructors().empty())
            {
                for (const auto& ctor : class_.constructors())
                {
                    out << "    .add_constructor<" << ctor.signature() << ">()\n";
                }

                out << "    .enable_garbage_collector()\n";
            }

            if (!class_.baseClasses().empty())
            {
                for (const auto& base : class_.baseClasses())
                {
                    out << "    .add_base_class<" << base << ">()\n";
                }
            }

            if (!class_.variables().empty())
            {
                for (const auto& var : class_.variables())
                {
                    if (!var.isStatic())
                    {
                        out << "    .add_variable(\"" << var.name() << "\", &" << class_.name() << "::" << var.name() << ")\n";
                    }
                    else
                    {
                        out << "    .add_variable(\"" << var.name() << "\", " << class_.name() << "::" << var.name() << ")\n";
                    }
                }
            }

            if (!class_.methods().empty())
            {
                for (const auto& method : class_.methods())
                {
                    if (method.isConst())
                    {
                        out << "    .add_method(\"" << method.name() << "\", static_cast<" << method.returnType() << "(" << class_.name() << "::*)" << method.signature() << " const>(&"
                            << class_.name() << "::" << method.name() << "))\n";
                    }
                    else
                    {
                        out << "    .add_method(" << "\"" << method.name() << "\", sol::resolve<" << method.returnType() << method.signature() << ">(&" << class_.name() << "::" << method.name()
                            << "))\n";
                    }
                }
            }

            out << ";\n";
        }

        out << "    }\n";
    }

    void SolGenerator::generate_global_functions(const std::vector<engine::meta::FunctionDescriptor>& global_functions, std::ofstream& out)
    {
        out << "    void register_global_functions(sol::state& lua)\n";
        out << "    {\n";

        for (const auto& fn : global_functions)
        {
            out << "   function_registrar(lua)\n";
            out << "       .add_function(\"" << fn.name() << "\", sol::resolve<" << fn.returnType() << fn.signature() << ">(&" << fn.name() << "))\n";
        }

        out << ";\n";

        out << "    }\n";
    }
} // namespace codegen