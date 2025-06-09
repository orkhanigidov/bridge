from pathlib import Path
from typing import List, Optional, Dict, Any

import yaml

from .clang_analyzer import LibClangAnalyzer, FunctionInfo
from .code_builder import CodeBuilder


class ReflectionGenerator:
    def __init__(self, include_paths: List[str]) -> None:
        self.analyzer = LibClangAnalyzer(include_paths)
        self.registered_types = set()

    def generate_from_config(self, config_file: str, output_file: Optional[str] = None) -> str:
        config = self._load_config(config_file)
        parsed_data = self._analyze_headers(config)
        code = self._generate_code(config, parsed_data)

        if output_file:
            self._write_output(code, output_file)

        return code

    def _load_config(self, config_file: str) -> Dict:
        try:
            with open(config_file, "r") as f:
                return yaml.safe_load(f)
        except Exception as e:
            raise RuntimeError(f"Failed to load config file {config_file}: {e}")

    def _analyze_headers(self, config: Dict) -> Dict[str, Any]:
        headers = config.get("headers", [])
        if not headers:
            raise ValueError("No headers specified in configuration")

        print(f"Analyzing headers: {headers}")
        parsed_data = self.analyzer.analyze_headers(headers, config)

        print(f'Found {len(parsed_data["functions"])} functions')
        print(f'Found {len(parsed_data["classes"])} classes')
        print(f'Found namespaces: {parsed_data["namespaces"]}')

        return parsed_data

    def _generate_code(self, config: Dict, parsed_data: Dict) -> str:
        builder = CodeBuilder()

        self._add_header(builder)
        self._add_includes(builder, config)
        self._add_registration_block(builder, config, parsed_data)

        return builder.build()

    def _add_header(self, builder: CodeBuilder) -> None:
        builder.line("// Auto-generated file. Do not edit manually!").line()

    def _add_includes(self, builder: CodeBuilder, config: Dict) -> None:
        builder.line('#include "reflection/ClassRegistrar.hpp"')
        builder.line('#include "reflection/GlobalMethodRegistrar.hpp"').line()
        builder.line("#include <rttr/registration>").line()

        for header in config.get("headers", []):
            builder.line(f"#include <{header}>")
        builder.line()

    def _add_registration_block(self, builder: CodeBuilder, config: Dict, parsed_data: Dict) -> None:
        with builder.block("RTTR_REGISTRATION {", "}"):
            self._add_using_statements(builder, config)
            self._add_class_registrations(builder, config)
            self._add_method_registrations(builder, config, parsed_data)
            self._add_function_registrations(builder, config, parsed_data)

    def _add_using_statements(self, builder: CodeBuilder, config: Dict) -> None:
        builder.line().line("using namespace engine::reflection;")

        for ns in config.get("namespaces", []):
            builder.line(f"using namespace {ns};")
        builder.line()

    def _add_function_registrations(self, builder: CodeBuilder, config: Dict, parsed_data: Dict) -> None:
        functions_config = config.get("functions", [])
        if not functions_config:
            return

        builder.line("// ==================== Global Method Registrations ====================").line()
        for func_config in functions_config:
            self._add_single_function(builder, func_config, parsed_data)
            builder.line()
        builder.line()

    def _add_class_registrations(self, builder: CodeBuilder, config: Dict) -> None:
        classes_config = config.get("classes", [])
        if not classes_config:
            return

        builder.line("// ==================== Class Registrations ====================")
        for class_config in classes_config:
            name = class_config["name"]
            alias = class_config.get("alias", name)
            builder.line(f'ClassRegistrar::register_class<{name}>("{alias}", "{name}");')
            self.registered_types.add(name)
        builder.line()

    def _add_method_registrations(self, builder: CodeBuilder, config: Dict, parsed_data: Dict) -> None:
        methods_config = config.get("methods", [])
        if not methods_config:
            return

        class_map = {c["name"]: c["name"] for c in config.get("classes", [])}

        builder.line("// ==================== Method Registrations ====================")
        for method_config in methods_config:
            self._add_single_method(builder, method_config, class_map, parsed_data)
            builder.line()
        builder.line()

    def _add_single_function(self, builder: CodeBuilder, func_config: Dict, parsed_data: Dict) -> None:
        func_name = func_config["name"]
        func_overloads = parsed_data["functions"].get(func_name, [])

        if not func_overloads:
            print(f"Warning: Function {func_name} not found in parsed data")
            return

        category = func_config.get("category", "")
        description = func_config.get("description", "")

        for i, func_data in enumerate(func_overloads):
            self._register_function_overload(builder, func_config, func_data, i, len(func_overloads), category,
                                             description)

    def _register_function_overload(self, builder: CodeBuilder, func_config: Dict, func_data: FunctionInfo, index: int,
                                    total_overloads: int, category: str, description: str) -> None:
        return_type = func_config.get("return_type", func_data.return_type)
        parameter_types = func_config.get("parameter_types", func_data.parameter_types)
        parameters = func_config.get("parameters", func_data.parameters)

        if not self._are_types_registered(return_type, parameter_types):
            print(f"Skipping function {func_data.name}: types not registered")
            return

        param_types_str = ", ".join(parameter_types)
        param_names_str = ", ".join(f'"{p}"' for p in parameters)

        overload_suffix = f"_{index + 1}" if total_overloads > 1 else ""
        registration_name = f"{func_data.name}{overload_suffix}"

        builder.line(f'GlobalMethodRegistrar::create("{registration_name}")')
        builder.indent()
        builder.line(f".method(rttr::select_overload<{return_type}({param_types_str})>(&{func_data.name}),")
        builder.line(f'"{category}", "{description}", {param_names_str});')
        builder.dedent()

    def _add_single_method(self, builder: CodeBuilder, method_config: Dict, class_map: Dict, parsed_data: Dict) -> None:
        class_name = class_map.get(method_config["class_name"])
        if not class_name:
            print(f'Warning: Class name {method_config["class_name"]} not found')
            return

        method_name = method_config["name"]
        method_overloads, static_method_overloads = self._find_method_in_class_hierarchy(class_name, method_name,
                                                                                         parsed_data)

        all_overloads = method_overloads + static_method_overloads
        if not all_overloads:
            print(f"Warning: Method {class_name}::{method_name} not found in parsed data")
            return

        category = method_config.get("category", "")
        description = method_config.get("description", "")

        for i, method_data in enumerate(all_overloads):
            self._register_method_overload(builder, method_config, method_data, class_name, i, len(all_overloads),
                                           category, description)

    def _register_method_overload(self, builder: CodeBuilder, method_config: Dict, method_data: FunctionInfo,
                                  class_name: str, index: int, total_overloads: int, category: str,
                                  description: str) -> None:
        return_type = method_config.get("return_type", method_data.return_type)
        parameter_types = method_config.get("parameter_types", method_data.parameter_types)
        parameters = method_config.get("parameters", method_data.parameters)

        if class_name not in self.registered_types or not self._are_types_registered(return_type, parameter_types):
            print(f"Skipping method {class_name}::{method_data.name}: types not registered")
            return

        param_types_str = ", ".join(parameter_types)
        param_names_str = ", ".join(f'"{p}"' for p in parameters)

        overload_suffix = f"_{index + 1}" if total_overloads > 1 else ""
        registration_name = f"{method_data.name}{overload_suffix}"

        builder.line(f"MethodRegistrar::register_member_method<{class_name}>(")
        builder.indent()
        builder.line(f'"{class_name}", "{registration_name}",')
        builder.line(f"rttr::select_overload<{return_type}({param_types_str})>(&{class_name}::{method_data.name}),")
        builder.line(f'"{category}", "{description}", {param_names_str});')
        builder.dedent()

    def _are_types_registered(self, return_type: str, parameter_types: List[str]) -> bool:
        primitive_types = {"void", "int", "float", "double", "bool", "char", "long", "short", "unsigned", "string",
                           "std::string", "std::vector", "std::ostream", "std::istream"}

        return_type_clean = return_type.replace("const", "").replace("&", "").replace("*", "").strip()
        if (return_type_clean not in primitive_types) and (return_type_clean not in self.registered_types):
            return False

        for param_type in parameter_types:
            param_type_clean = param_type.replace("const", "").replace("&", "").replace("*", "").strip()
            if (param_type_clean not in primitive_types) and (param_type_clean not in self.registered_types):
                return False

        return True

    def _find_method_in_class_hierarchy(self, class_name: str, method_name: str, parsed_data: Dict,
                                        visited: Optional[set] = None) -> tuple[List[FunctionInfo], List[FunctionInfo]]:
        if visited is None:
            visited = set()

        if class_name in visited:
            return [], []

        visited.add(class_name)

        class_data = parsed_data["classes"].get(class_name)
        if not class_data:
            return [], []

        method_overloads = class_data.methods.get(method_name, [])
        static_method_overloads = class_data.static_methods.get(method_name, [])

        for base_class in class_data.base_classes:
            base_method, base_static = self._find_method_in_class_hierarchy(base_class, method_name, parsed_data,
                                                                            visited)
            method_overloads.extend(base_method)
            static_method_overloads.extend(base_static)

        return method_overloads, static_method_overloads

    def _write_output(self, code: str, output_file: str) -> None:
        output_path = Path(output_file)
        output_path.parent.mkdir(parents=True, exist_ok=True)

        try:
            with open(output_path, "w") as f:
                f.write(code)
            print(f"Generated: {output_file}")
        except Exception as e:
            raise RuntimeError(f"Failed to write output file {output_file}: {e}")
