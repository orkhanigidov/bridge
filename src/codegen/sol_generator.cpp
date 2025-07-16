#include "codegen/sol_generator.hpp"

namespace codegen
{
    SolGenerator::SolGenerator(const std::string& output_filename) : output_filename_(output_filename) {}

    void SolGenerator::generate(const std::vector<engine::meta::ClassDescriptor>& classes, const std::vector<engine::meta::FunctionDescriptor>& global_functions)
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
            out << "    class_registrar<" << class_.name() << ">(lua, \"" << class_.name() << "\")\n";

            if (!class_.constructors().empty())
            {
                for (const auto& ctor : class_.constructors())
                {
                    out << "    .add_constructor<" << ctor.signature() << ">()\n";
                }
            }

            if (!class_.methods().empty())
            {
                for (const auto& method : class_.methods())
                {
                    out << "    .add_method(" << "\"" << method.name() << "\", sol::resolve<" << method.returnType() << method.signature() << ">(&" << class_.name() << "::" << method.name() << "))\n";
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