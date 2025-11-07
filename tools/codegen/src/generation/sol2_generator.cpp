/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file sol2_generator.cpp
 * @brief Implements the Sol2Generator for generating Lua bindings using Sol2.
 */

#include "generation/sol2_generator.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "metadata/class_descriptor.hpp"
#include "metadata/enum_descriptor.hpp"
#include "metadata/function_descriptor.hpp"

namespace
{
    /**
     * @brief Writes a line to the output file with the specified indentation and newlines.
     * @param out Output file stream.
     * @param indent Number of indentation levels (4 spaces each).
     * @param text The text to write.
     * @param newlines Number of newline characters to append (default: 1).
     */
    void write_line(std::ofstream& out, int indent, std::string_view text, int newlines = 1)
    {
        out << std::string(static_cast<size_t>(indent) * 4, ' ') << text << std::string(static_cast<size_t>(std::max(0, newlines)), '\n');
    }

    /**
     * @brief Formats a string to a valid Lua identifier.
     *
     * Replaces non-alphanumeric characters with underscores, removes leading/trailing underscores, collapses consecutive underscores, and converts to camel case.
     *
     * @param name The string to format.
     * @return The formatted Lua identifier.
     */
    std::string format_lua_name(std::string name)
    {
        std::ranges::for_each(name, [](char& c)
        {
            if (!std::isalnum(static_cast<unsigned char>(c)))
            {
                c = '_';
            }
        });

        auto new_end = std::ranges::unique(name, [](char a, char b) { return a == '_' && b == '_'; }).begin();
        name.erase(new_end, name.end());

        if (name.starts_with('_'))
        {
            name.erase(0, 1);
        }

        if (name.ends_with('_'))
        {
            name.pop_back();
        }

        std::string result;
        for (size_t i = 0; i < name.size(); ++i)
        {
            if (name[i] == '_')
            {
                if (i + 1 < name.size())
                {
                    result += std::toupper(static_cast<unsigned char>(name[i + 1]));
                    ++i;
                }
            }
            else
            {
                result += name[i];
            }
        }
        name = result;

        return name;
    }

    /**
     * @brief Writes Sol2 overload resolution expressions for a set of functions.
     * @tparam IsMember True if the functions are class members, false for free functions.
     * @param stream Output string stream.
     * @param funcs Vector of function descriptors.
     * @param indent indent Indentation level.
     * @param class_name Name of the class (for member functions).
     */
    template <bool IsMember>
    void write_overload_resolutions(std::ostringstream& stream, const std::vector<const codegen::metadata::FunctionDescriptor*>& funcs, int indent, std::string_view class_name = "")
    {
        const std::string indent_str(static_cast<size_t>(indent) * 4, ' ');
        const std::string delimiter = ",\n" + indent_str;

        for (size_t i = 0; i < funcs.size(); ++i)
        {
            const auto* func = funcs[i];
            if constexpr (IsMember)
            {
                stream << std::format("sol::resolve<{}{}{}>(&{}::{})",
                                      func->return_type_name(),
                                      func->signature(),
                                      func->is_const() ? " const" : "",
                                      class_name,
                                      func->name());
            }
            else
            {
                stream << std::format("sol::resolve<{}{}>(&{})",
                                      func->return_type_name(),
                                      func->signature(),
                                      func->name());
            }

            if (i < funcs.size() - 1)
            {
                stream << delimiter;
            }
        }
    }

    /**
     * @brief Writes enum name-value pairs for Sol2 registration.
     * @param stream Output string stream.
     * @param enum_ Enum descriptor.
     * @param indent Indentation level.
     * @param full_enum_name Fully qualified enum name.
     */
    void write_enum_pairs(std::ostringstream& stream, const codegen::metadata::EnumDescriptor& enum_, int indent, std::string_view full_enum_name)
    {
        const std::string indent_str(static_cast<size_t>(indent) * 4, ' ');
        const std::string delimiter = ",\n" + indent_str;
        const auto& enumerators = enum_.enumerators();

        for (size_t i = 0; i < enumerators.size(); ++i)
        {
            stream << std::format("{{\"{}\", {}::{}}}", enumerators[i].name, full_enum_name, enumerators[i].name);
            if (i < enumerators.size() - 1)
            {
                stream << delimiter;
            }
        }
    }
}

namespace codegen::generation
{
    /**
     * @brief Generates Sol2 bindings for the provided metadata.
     * @param includes Set of header files to include.
     * @param namespaces Set of C++ namespaces to process.
     * @param classes List of class descriptors to generate bindings for.
     * @param free_functions List of free function descriptors to bind.
     * @param enums List of enum descriptors to bind.
     */
    void Sol2Generator::generate(const std::unordered_set<std::string>& includes,
                                 const std::unordered_set<std::string>& namespaces,
                                 const std::vector<metadata::ClassDescriptor>& classes,
                                 const std::vector<metadata::FunctionDescriptor>& free_functions,
                                 const std::vector<metadata::EnumDescriptor>& enums) const
    {
        std::unordered_set<std::string> container_types;
        for (const auto& cls : classes)
        {
            for (const auto& var : cls.member_variables())
            {
                if (var.is_container())
                {
                    container_types.emplace(var.type_name());
                }
            }
        }

        std::ofstream out(output_file_);
        if (!out.is_open())
        {
            throw Sol2GeneratorException(std::format("Failed to open output file: {}", output_file_.string()));
        }

        write_header(out, includes, namespaces, container_types);
        write_member_registrations(out, classes);
        write_non_member_registrations(out, free_functions, enums);
        write_footer(out);
    }

    /**
     * @brief Writes the file header, including and namespace declarations.
     * @param out Output file stream.
     * @param includes Set of header files to include.
     * @param namespaces Set of namespaces to open.
     * @param container_types Set of container types to register.
     */
    void Sol2Generator::write_header(std::ofstream& out,
                                     const std::unordered_set<std::string>& includes,
                                     const std::unordered_set<std::string>& namespaces,
                                     const std::unordered_set<std::string>& container_types)
    {
        write_line(out, 0, "// This file is auto-generated by the code generator. Do not edit manually!", 2);
        write_line(out, 0, "#include \"bindings/lua/lua_binder.hpp\"");
        write_line(out, 0, "#include \"bindings/lua/member_registrar.hpp\"");
        write_line(out, 0, "#include \"bindings/lua/non_member_registrar.hpp\"", 2);

        for (const auto& include : includes)
        {
            write_line(out, 0, std::format("#include <{}>", include));
        }
        write_line(out, 0, "");
        write_line(out, 0, "#include <sol/sol.hpp>", 2);

        for (const auto& ns : namespaces)
        {
            write_line(out, 0, std::format("using namespace {};", ns));
        }
        write_line(out, 0, "using namespace codegen::wrappers;", 2);

        write_line(out, 0, "namespace sol");
        write_line(out, 0, "{");
        for (const auto& type_name : container_types)
        {
            write_line(out, 1, std::format("template <>"));
            write_line(out, 1, std::format("struct is_container<{}> : std::false_type {{}};", type_name), 2);
        }
        write_line(out, 0, "}", 2);

        write_line(out, 0, "namespace engine::bindings::lua");
        write_line(out, 0, "{");
    }

    /**
     * @brief Writes Sol2 member registrations for all classes.
     * @param out Output file stream.
     * @param classes List of class descriptors.
     */
    void Sol2Generator::write_member_registrations(std::ofstream& out, const std::vector<metadata::ClassDescriptor>& classes)
    {
        write_line(out, 1, "void register_members(sol::state& lua)");
        write_line(out, 1, "{");

        std::unordered_set<std::string> target_class_names;
        for (const auto& cls : classes)
        {
            target_class_names.emplace(cls.name());
        }

        std::unordered_set<std::string> registered_bases;
        for (const auto& cls : classes)
        {
            for (const auto& base_name : cls.base_class_names())
            {
                if (target_class_names.contains(base_name))
                {
                    continue;
                }

                if (registered_bases.emplace(base_name).second)
                {
                    std::string stripped_base_name = base_name;
                    if (size_t template_bracket_pos = stripped_base_name.find('<'); template_bracket_pos != std::string::npos)
                    {
                        stripped_base_name = stripped_base_name.substr(0, template_bracket_pos);
                    }
                    if (size_t ns_pos = stripped_base_name.rfind("::"); ns_pos != std::string::npos)
                    {
                        stripped_base_name = stripped_base_name.substr(ns_pos + 2);
                    }
                    std::string lua_base_name = format_lua_name(stripped_base_name);
                    write_line(out, 2, std::format("MemberRegistrar<{}, MemoryOwnership::Cpp>(lua, \"{}\");", base_name, lua_base_name));
                }
            }
        }

        if (!registered_bases.empty())
        {
            write_line(out, 0, "");
        }

        for (const auto& cls : classes)
        {
            std::string lua_name = format_lua_name(cls.name());
            std::string ownership;
            if (cls.constructors().empty())
            {
                ownership = "Cpp";
            }
            else if (!cls.base_class_names().empty())
            {
                ownership = "Cpp";
            }
            else
            {
                ownership = "Lua";
            }

            write_line(out, 2, std::format("MemberRegistrar<{}, MemoryOwnership::{}>(lua, \"{}\")", cls.name(), ownership, lua_name));

            if (!cls.constructors().empty())
            {
                std::ostringstream constructor_stream;
                const auto& ctors = cls.constructors();
                for (size_t i = 0; i < ctors.size(); ++i)
                {
                    std::string signature = ctors[i].signature();
                    if (signature.starts_with('(') && signature.ends_with(')'))
                    {
                        signature = signature.substr(1, signature.length() - 2);
                    }
                    constructor_stream << std::format("sol::types<{}>", signature);
                    if (i < ctors.size() - 1)
                    {
                        constructor_stream << ", ";
                    }
                }

                if (ownership == "Lua")
                {
                    write_line(out, 3, std::format(".add_shared_constructors<{}>()", constructor_stream.str()));
                }
                else // if (ownership == "Cpp")
                {
                    write_line(out, 3, std::format(".add_raw_constructors<{}>()", constructor_stream.str()));
                }
            }

            if (!cls.base_class_names().empty())
            {
                std::ostringstream base_stream;
                const auto& bases = cls.base_class_names();
                for (size_t i = 0; i < cls.base_class_names().size(); ++i)
                {
                    base_stream << bases[i];
                    if (i < bases.size() - 1)
                    {
                        base_stream << ", ";
                    }
                }
                write_line(out, 3, std::format(".add_bases<{}>()", base_stream.str()));
            }

            for (const auto& enum_ : cls.member_enumerators())
            {
                std::ostringstream enumerators_stream;
                std::string full_enum_name = std::format("{}::{}", cls.name(), enum_.name());
                write_enum_pairs(enumerators_stream, enum_, 4, full_enum_name);
                write_line(out, 3, std::format(".add_enums<{}::{}>(\"{}\", {{ {} }})", cls.name(), enum_.name(), enum_.name(), enumerators_stream.str()));
            }

            for (const auto& var : cls.member_variables())
            {
                if (var.is_static())
                {
                    write_line(out, 3, std::format(".add_static_variable(\"{}\", {}::{})", var.name(), cls.name(), var.name()));
                }
                else if (var.is_container())
                {
                    write_line(out, 3, std::format(".add_readonly_property(\"{}\", [](const {}& self) {{ return self.{}; }})", var.name(), cls.name(), var.name()));
                }
                else
                {
                    write_line(out, 3, std::format(".add_variable(\"{}\", &{}::{}, {})", var.name(), cls.name(), var.name(), var.is_const()));
                }
            }

            std::unordered_map<std::string, std::vector<const metadata::FunctionDescriptor*>> overloads;
            for (const auto& func : cls.member_functions())
            {
                overloads[func.name()].emplace_back(&func);
            }

            for (const auto& [name, funcs] : overloads)
            {
                std::ostringstream overloads_stream;
                if (funcs.size() == 1)
                {
                    overloads_stream << std::format("&{}::{}", cls.name(), funcs[0]->name());
                }
                else
                {
                    write_overload_resolutions<true>(overloads_stream, funcs, 4, cls.name());
                }
                write_line(out, 3, std::format(".add_function(\"{}\", {})", name, overloads_stream.str()));
            }
            write_line(out, 2, ";", 2);
        }
        write_line(out, 1, "}", 2);
    }

    /**
     * @brief Writes Sol2 registrations for free functions and enums.
     * @param out Output file stream.
     * @param free_functions List of free function descriptors.
     * @param enums List of enum descriptors.
     */
    void Sol2Generator::write_non_member_registrations(std::ofstream& out,
                                                       const std::vector<metadata::FunctionDescriptor>& free_functions,
                                                       const std::vector<metadata::EnumDescriptor>& enums)
    {
        write_line(out, 1, "void register_non_members(sol::state& lua)");
        write_line(out, 1, "{");
        write_line(out, 2, "NonMemberRegistrar registrar(lua);", 2);

        std::unordered_map<std::string, std::vector<const metadata::FunctionDescriptor*>> overloads;
        for (const auto& func : free_functions)
        {
            overloads[func.name()].emplace_back(&func);
        }

        for (const auto& [name, funcs] : overloads)
        {
            std::ostringstream overloads_stream;
            if (funcs.size() == 1)
            {
                overloads_stream << std::format("&{}", name);
            }
            else
            {
                write_overload_resolutions<false>(overloads_stream, funcs, 4);
            }
            write_line(out, 2, std::format("registrar.add_function(\"{}\", {});", name, overloads_stream.str()));
        }

        for (const auto& enum_ : enums)
        {
            std::ostringstream enumerators_stream;
            write_enum_pairs(enumerators_stream, enum_, 4, enum_.name());
            write_line(out, 2, std::format("registrar.add_enums<{}>(\"{}\", {{ {} }});", enum_.name(), enum_.name(), enumerators_stream.str()));
        }
        write_line(out, 1, "}");
    }

    /**
     * @brief Writes the file footer, closing any open namespaces.
     * @param out Output file stream.
     */
    void Sol2Generator::write_footer(std::ofstream& out)
    {
        write_line(out, 0, "} // namespace engine::bindings::lua");
    }
} // namespace codegen::generation
