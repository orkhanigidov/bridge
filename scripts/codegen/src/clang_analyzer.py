import dataclasses
import os
from typing import List, Dict, Optional, Any, Set

import clang.cindex


@dataclasses.dataclass
class FunctionInfo:
    name: str
    return_type: str
    parameters: List[str]
    parameter_types: List[str]
    signature: str
    is_static: bool
    location: str


@dataclasses.dataclass
class ClassInfo:
    name: str
    methods: Dict[str, List[FunctionInfo]]
    static_methods: Dict[str, List[FunctionInfo]]
    base_classes: List[str]


class LibClangAnalyzer:
    def __init__(self, include_paths: List[str]) -> None:
        self.include_paths = include_paths
        self._setup_clang()
        self.index = clang.cindex.Index.create()

    def analyze_headers(self, headers: List[str], config: Optional[Dict] = None) -> Dict[str, Any]:
        result = {
            'functions': {},
            'classes': {},
            'namespaces': set()
        }

        wanted_functions = self._get_wanted_functions(config)
        wanted_methods = self._get_wanted_methods(config)

        for header in headers:
            try:
                self._analyze_single_header(header, result, wanted_functions, wanted_methods)
            except Exception as e:
                print(f"Warning: Could not parse {header}: {e}")

        return result

    def _setup_clang(self) -> None:
        library_name = "libclang.dll" if os.name == "nt" else "libclang.so"
        library_path = os.path.join(
            os.path.dirname(clang.cindex.__file__),
            'native',
            library_name)
        clang.cindex.conf.set_library_file(library_path)

    def _get_wanted_functions(self, config: Optional[Dict]) -> Set[str]:
        if not config:
            return set()
        return {func["name"] for func in config.get("functions", [])}

    def _get_wanted_methods(self, config: Optional[Dict]) -> Dict[str, Set[str]]:
        if not config:
            return {}

        wanted_methods = {}
        for method in config.get("methods", []):
            class_name = method["class_name"]
            method_name = method["name"]

            if class_name not in wanted_methods:
                wanted_methods[class_name] = set()
            wanted_methods[class_name].add(method_name)

        return wanted_methods

    def _analyze_single_header(self, header: str, result: Dict, wanted_functions: Set[str],
                               wanted_methods: Dict[str, Set[str]]) -> None:
        header_path = self._find_header_path(header)
        if not header_path:
            raise FileNotFoundError(f"Header {header} not found in include paths")

        args = self._get_compile_args()
        tu = self.index.parse(header_path, args=args)
        self._check_diagnostics(tu, header)
        self._extract_from_cursor(tu.cursor, result, wanted_functions, wanted_methods)

    def _find_header_path(self, header: str) -> Optional[str]:
        for include_path in self.include_paths:
            potential_path = os.path.join(include_path, header)
            if os.path.exists(potential_path):
                return potential_path
        return None

    def _get_compile_args(self) -> List[str]:
        args = ["-x", "c++", "-std=c++17"]

        if os.name != "nt":
            args.extend(["-I/usr/include", "-I/usr/local/include"])

        for path in self.include_paths:
            args.append(f"-I{os.path.abspath(path)}")

        return args

    def _check_diagnostics(self, tu, header: str) -> None:
        for diag in tu.diagnostics:
            if diag.severity >= clang.cindex.Diagnostic.Error:
                print(f"Error in {header}: {diag.spelling}")

    def _extract_from_cursor(self, cursor, result: Dict, wanted_functions: Set[str],
                             wanted_methods: Dict[str, Set[str]]) -> None:
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.NAMESPACE:
                result["namespaces"].add(child.spelling)
                self._extract_from_cursor(child, result, wanted_functions, wanted_methods)

            elif child.kind == clang.cindex.CursorKind.FUNCTION_DECL:
                self._process_function(child, result, wanted_functions)

            elif child.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
                self._process_class(child, result, wanted_methods)

            elif child.kind == clang.cindex.CursorKind.CLASS_TEMPLATE:
                self._extract_from_cursor(child, result, wanted_functions, wanted_methods)

    def _process_function(self, cursor, result: Dict, wanted_functions: Set[str]) -> None:
        if not self._is_public_function(cursor):
            return

        func_info = self._extract_function_info(cursor)
        if not func_info:
            return

        func_name = func_info.name
        if wanted_functions and func_name not in wanted_functions:
            return

        if func_name not in result["functions"]:
            result["functions"][func_name] = []

        existing_signatures = {f.signature for f in result["functions"][func_name]}
        if func_info.signature not in existing_signatures:
            result["functions"][func_name].append(func_info)
            print(f"Found function: {func_info.signature}")

    def _process_class(self, cursor, result: Dict, wanted_methods: Dict[str, Set[str]]) -> None:
        class_info = self._extract_class_info(cursor, wanted_methods)

        if not class_info:
            return

        if class_info:
            result["classes"][class_info.name] = class_info

    def _is_public_function(self, cursor) -> bool:
        return (cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC or
                cursor.linkage == clang.cindex.LinkageKind.EXTERNAL)

    def _is_public_method(self, cursor) -> bool:
        return cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC

    def _extract_function_info(self, cursor) -> Optional[FunctionInfo]:
        try:
            params = list(cursor.get_arguments())

            is_class_method = False
            class_name = ""
            nested_types = set()

            if cursor.semantic_parent and cursor.semantic_parent.kind in [clang.cindex.CursorKind.CLASS_DECL,
                                                                          clang.cindex.CursorKind.STRUCT_DECL]:
                is_class_method = True
                class_name = cursor.semantic_parent.spelling
                nested_types = self._find_nested_types(cursor.semantic_parent)

            parameter_types = []
            for param in params:
                param_type = param.type.spelling

                if is_class_method and "::" not in param_type:
                    base_type = param_type.split()[0]
                    if base_type in nested_types:
                        param_type = param_type.replace(base_type, f"{class_name}::{base_type}", 1)

                parameter_types.append(param_type)

            return FunctionInfo(
                name=cursor.spelling,
                return_type=cursor.result_type.spelling,
                parameters=[p.spelling or f"param{i}" for i, p in enumerate(params)],
                parameter_types=parameter_types,
                signature=self._generate_signature(cursor),
                is_static=cursor.storage_class == clang.cindex.StorageClass.STATIC,
                location=f"{cursor.location.file}:{cursor.location.line}" if cursor.location.file else "unknown"
            )
        except Exception as e:
            print(f"Error extracting function {cursor.spelling}: {e}")
            return None

    def _find_nested_types(self, cursor) -> Set[str]:
        nested_types = set()

        for child in cursor.get_children():
            if child.kind in [clang.cindex.CursorKind.TYPEDEF_DECL, clang.cindex.CursorKind.TYPE_ALIAS_DECL,
                              clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL,
                              clang.cindex.CursorKind.ENUM_DECL]:
                nested_types.add(child.spelling)

        return nested_types

    def _generate_signature(self, cursor) -> str:
        params = list(cursor.get_arguments())
        param_types = [p.type.spelling for p in params]
        return f'{cursor.spelling}({", ".join(param_types)})'

    def _extract_class_info(self, cursor, wanted_methods: Dict[str, Set[str]]) -> Optional[ClassInfo]:
        try:
            methods = {}
            static_methods = {}
            base_classes = []
            class_name = cursor.spelling

            for child in cursor.get_children():
                if child.kind == clang.cindex.CursorKind.CXX_METHOD:
                    self._process_method(child, class_name, methods, static_methods, wanted_methods)
                elif child.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER:
                    base_type = child.get_definition()
                    if base_type:
                        base_classes.append(base_type.spelling)

            return ClassInfo(
                name=class_name,
                methods=methods,
                static_methods=static_methods,
                base_classes=base_classes
            )
        except Exception as e:
            print(f"Error extracting class {cursor.spelling}: {e}")
            return None

    def _process_method(self, cursor, class_name: str, methods: Dict, static_methods: Dict,
                        wanted_methods: Dict[str, Set[str]]) -> None:
        if not self._is_public_method(cursor):
            return

        method_info = self._extract_function_info(cursor)
        if not method_info:
            return

        method_name = method_info.name
        if wanted_methods and class_name in wanted_methods and method_name not in wanted_methods[class_name]:
            return

        target_dict = static_methods if cursor.is_static_method() else methods

        if method_name not in target_dict:
            target_dict[method_name] = []
        target_dict[method_name].append(method_info)

        print(f"Found method: {class_name}::{method_info.signature}")
