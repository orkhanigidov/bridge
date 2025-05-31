import yaml
from pathlib import Path
from typing import Dict, List, Any
from .code_builder import CodeBuilder
from .clang_analyzer import LibClangAnalyzer


class ReflectionGenerator:
    def __init__(self, include_paths):
        self.analyzer = LibClangAnalyzer(include_paths)

    def generate_from_config(self, config_file, output_file):
        config = self._load_config(config_file)
        parsed_data = self._analyze_headers(config)
        code = self._generate_code(config, parsed_data)

        if output_file:
            self._write_output(code, output_file)

        return code

    def _load_config(self, config_file):
        with open(config_file, 'r') as f:
            return yaml.safe_load(f)

    def _analyze_headers(self, config):
        headers = config.get('headers', [])
        print(f"Analyzing headers: {headers}")

        parsed_data = self.analyzer.analyze_headers(headers)

        print(f"Found {len(parsed_data['functions'])} functions")
        print(f"Found {len(parsed_data['classes'])} classes")
        print(f"Found namespaces: {parsed_data['namespaces']}")

        return parsed_data

    def _generate_code(self, config, parsed_data):
        builder = CodeBuilder()

        self._add_header(builder)
        self._add_includes(builder, config)
        self._add_registration_block(builder, config, parsed_data)

        return builder.build()

    def _add_header(self, builder):
        builder.line("// Auto-generated file. Do not edit manually")
        builder.line()

    def _add_includes(self, builder, config):
        builder.line('#include "reflection/MethodRegistrar.hpp"')
        builder.line('#include "reflection/TypeRegistrar.hpp"')
        builder.line()

        for header in config.get('headers', []):
            builder.line(f"#include <{header}>")
        builder.line('#include <rttr/registration>')
        builder.line()

    def _add_registration_block(self, builder, config, parsed_data):
        with builder.block("RTTR_REGISTRATION", "}"):
            self._add_using_statements(builder, config)
            self._add_type_registrations(builder, config)
            self._add_function_registrations(builder, config, parsed_data)
            self._add_method_registrations(builder, config, parsed_data)

    def _add_using_statements(self, builder, config):
        builder.line("using namespace engine::reflection;")
        for ns in config.get('namespaces', []):
            builder.line(f"using namespace {ns};")
        builder.line()

    def _add_type_registrations(self, builder, config):
        types = config.get('types', [])
        if not types:
            return

        for type_info in types:
            name = type_info['name']
            alias = type_info.get('alias', name)
            builder.line(
                f'TypeRegistrar::register_type<{name}>("{alias}", "{name}");')

        builder.line()

    def _add_function_registrations(self, builder, config, parsed_data):
        functions_config = config.get('functions', [])
        if not functions_config:
            return

        for func_config in functions_config:
            self._add_single_function(builder, func_config, parsed_data)

        builder.line()

    def _add_method_registrations(self, builder, config, parsed_data):
        methods_config = config.get('methods', [])
        if not methods_config:
            return

        type_map = {t['id']: t['name'] for t in config.get('types', [])}

        for method_config in methods_config:
            self._add_single_method(
                builder, method_config, type_map, parsed_data)

    def _add_single_function(self, builder, func_config, parsed_data):
        func_name = func_config['name']
        func_data = parsed_data['functions'].get(func_name)

        if not func_data:
            print(f"Warning: Function {func_name} not found in parsed data")
            return

        return_type = func_config.get('return_type', func_data['return_type'])
        parameter_types = func_config.get(
            'parameter_types', func_data['parameter_types'])
        parameters = func_config.get('parameters', func_data['parameters'])

        param_types_str = ', '.join(parameter_types)
        param_names_str = ', '.join(f'"{p}"' for p in parameters)

        builder.line('MethodRegistrar::register_global_method(')
        builder.indent()
        builder.line(
            f'"{func_name}", rttr::select_overload<{return_type}({param_types_str})>(&{func_name}), "",')
        builder.line(f'"", {param_names_str});')
        builder.dedent()

    def _add_single_method(self, builder, method_config, type_map, parsed_data):
        class_name = type_map.get(method_config['type_id'])
        if not class_name:
            print(f"Warning: Type ID {method_config['type_id']} not found")
            return

        method_name = method_config['name']
        class_data = parsed_data['classes'].get(class_name)

        if not class_data:
            print(f"Warning: Class {class_name} not found in parsed data")
            return

        method_data = (class_data['methods'].get(method_name) or
                       class_data['static_methods'].get(method_name))

        if not method_data:
            print(
                f"Warning: Method {class_name}::{method_name} not found in parsed data")
            return

        return_type = method_config.get(
            'return_type', method_data['return_type'])
        parameter_types = method_config.get(
            'parameter_types', method_data['parameter_types'])
        parameters = method_config.get('parameters', method_data['parameters'])

        param_types_str = ', '.join(parameter_types)
        param_names_str = ', '.join(f'"{p}"' for p in parameters)

        builder.line(f'MethodRegistrar::register_member_method<{class_name}>(')
        builder.indent()
        builder.line(f'"{class_name}", "{method_name}",')
        builder.line(
            f'rttr::select_overload<{return_type}({param_types_str})>')
        builder.line(f'(&{class_name}::{method_name}),')
        builder.line(f'"", "", {param_names_str});')
        builder.dedent()

    def _write_output(self, code, output_file):
        Path(output_file).parent.mkdir(parents=True, exist_ok=True)
        with open(output_file, 'w') as f:
            f.write(code)
        print(f"Generated: {output_file}")
