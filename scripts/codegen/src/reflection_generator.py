from pathlib import Path

import yaml

from .clang_analyzer import LibClangAnalyzer
from .code_builder import CodeBuilder


def _load_config(config_file):
    with open(config_file, "r") as f:
        return yaml.safe_load(f)


def _add_header(builder):
    builder.line("// Auto-generated file. Do not edit manually!")
    builder.line()


def _add_includes(builder, config):
    builder.line('#include "reflection/MethodRegistrar.hpp"')
    builder.line('#include "reflection/ClassRegistrar.hpp"')
    builder.line()

    for header in config.get("headers", []):
        builder.line(f"#include <{header}>")

    builder.line("#include <rttr/registration>")
    builder.line()


def _add_using_statements(builder, config):
    builder.line("using namespace engine::reflection;")

    for ns in config.get("namespaces", []):
        builder.line(f"using namespace {ns};")

    builder.line()


def _add_class_registrations(builder, config):
    classes_config = config.get("classes", [])

    if not classes_config:
        return

    for class_config in classes_config:
        name = class_config["name"]
        alias = class_config.get("alias", name)
        builder.line(f'ClassRegistrar::register_class<{name}>("{alias}", "{name}");')

    builder.line()


def _add_single_function(builder, func_config, parsed_data):
    func_name = func_config["name"]
    func_data = parsed_data["functions"].get(func_name)

    if not func_data:
        print(f"Warning: Function {func_name} not found in parsed data")
        return

    return_type = func_config.get("return_type", func_data["return_type"])
    parameter_types = func_config.get("parameter_types", func_data["parameter_types"])
    parameters = func_config.get("parameters", func_data["parameters"])
    category = func_config.get("category", "")
    description = func_config.get("description", "")

    param_types_str = ", ".join(parameter_types)
    param_names_str = ", ".join(f'"{p}"' for p in parameters)

    builder.line("MethodRegistrar::register_global_method(")
    builder.indent()
    builder.line(f'"{func_name}", rttr::select_overload<{return_type}({param_types_str})>(&{func_name}),')
    builder.line(f'"{category}", "{description}", {param_names_str});')
    builder.dedent()


def _add_single_method(builder, method_config, class_map, parsed_data):
    class_name = class_map.get(method_config["class_name"])

    if not class_name:
        print(f"Warning: Class ID {method_config["class_name"]} not found")
        return

    method_name = method_config["name"]
    class_data = parsed_data["classes"].get(class_name)

    if not class_data:
        print(f"Warning: Class {class_name} not found in parsed data")
        return

    method_data = (class_data["methods"].get(method_name) or class_data["static_methods"].get(method_name))

    if not method_data:
        print(f"Warning: Method {class_name}::{method_name} not found in parsed data")
        return

    return_type = method_config.get("return_type", method_data["return_type"])
    parameter_types = method_config.get("parameter_types", method_data["parameter_types"])
    parameters = method_config.get("parameters", method_data["parameters"])
    category = method_config.get("category", "")
    description = method_config.get("description", "")

    param_types_str = ", ".join(parameter_types)
    param_names_str = ", ".join(f'"{p}"' for p in parameters)

    builder.line(f"MethodRegistrar::register_member_method<{class_name}>(")
    builder.indent()
    builder.line(f'"{class_name}", "{method_name}",')
    builder.line(f"rttr::select_overload<{return_type}({param_types_str})>")
    builder.line(f"(&{class_name}::{method_name}),")
    builder.line(f'"{category}", "{description}", {param_names_str});')
    builder.dedent()


def _write_output(code, output_file):
    Path(output_file).parent.mkdir(parents=True, exist_ok=True)
    with open(output_file, "w") as f:
        f.write(code)
    print(f"Generated: {output_file}")


def _add_method_registrations(builder, config, parsed_data):
    methods_config = config.get("methods", [])

    if not methods_config:
        return

    class_map = {c["name"]: c["name"] for c in config.get("classes", [])}

    for method_config in methods_config:
        _add_single_method(builder, method_config, class_map, parsed_data)


def _add_function_registrations(builder, config, parsed_data):
    functions_config = config.get("functions", [])

    if not functions_config:
        return

    for func_config in functions_config:
        _add_single_function(builder, func_config, parsed_data)

    builder.line()


def _add_registration_block(builder, config, parsed_data):
    with builder.block("RTTR_REGISTRATION {", "}"):
        _add_using_statements(builder, config)
        _add_function_registrations(builder, config, parsed_data)
        _add_class_registrations(builder, config)
        _add_method_registrations(builder, config, parsed_data)


def _generate_code(config, parsed_data):
    builder = CodeBuilder()

    _add_header(builder)
    _add_includes(builder, config)
    _add_registration_block(builder, config, parsed_data)

    return builder.build()


class ReflectionGenerator:
    def __init__(self, include_paths):
        self.analyzer = LibClangAnalyzer(include_paths)

    def generate_from_config(self, config_file, output_file):
        config = _load_config(config_file)
        parsed_data = self._analyze_headers(config)
        code = _generate_code(config, parsed_data)

        if output_file:
            _write_output(code, output_file)

        return code

    def _analyze_headers(self, config):
        headers = config.get("headers", [])
        print(f"Analyzing headers: {headers}")

        parsed_data = self.analyzer.analyze_headers(headers)
        print(f"Found {len(parsed_data["functions"])} functions")
        print(f"Found {len(parsed_data["classes"])} classes")
        print(f"Found namespaces: {parsed_data["namespaces"]}")

        return parsed_data
