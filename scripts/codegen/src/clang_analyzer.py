import os
from pathlib import Path

import clang.cindex


def _setup_clang():
    if os.name == "nt":
        library_path = os.path.join(os.path.dirname(clang.cindex.__file__), 'native', 'libclang.dll')
    else:
        library_path = os.path.join(os.path.dirname(clang.cindex.__file__), 'native', 'libclang.so')

    clang.cindex.conf.set_library_file(library_path)


def _is_public_function(cursor):
    return cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC or cursor.linkage == clang.cindex.LinkageKind.EXTERNAL


def _is_public_method(cursor):
    return cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC


def _extract_function_info(cursor):
    try:
        params = list(cursor.get_arguments())
        return {
            "name": cursor.spelling,
            "return_type": cursor.result_type.spelling,
            "parameters": [p.spelling or f"param{i}" for i, p in enumerate(params)],
            "parameter_types": [p.type.spelling for p in params],
            "is_static": cursor.storage_class == clang.cindex.StorageClass.STATIC,
            "location": f"{cursor.location.file}:{cursor.location.line}" if cursor.location.file else "unknown"
        }

    except Exception as e:
        print(f"Error extracting function {cursor.spelling}: {e}")
        return None


def _extract_class_info(cursor):
    try:
        methods = {}
        static_methods = {}
        base_classes = []

        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.CXX_METHOD:
                method_info = _extract_function_info(child)
                if method_info and _is_public_method(child):
                    if child.is_static_method():
                        static_methods[method_info["name"]] = method_info
                    else:
                        methods[method_info["name"]] = method_info
            elif child.kind == clang.cindex.CursorKind.CXX_BASE_SPECIFIER:
                base_type = child.get_definition()
                if base_type:
                    base_classes.append(base_type.spelling)

        return {
            "name": cursor.spelling,
            "methods": methods,
            "static_methods": static_methods,
            "base_classes": base_classes
        }

    except Exception as e:
        print(f"Error extracting class {cursor.spelling}: {e}")
        return None


class LibClangAnalyzer:
    def __init__(self, include_paths):
        self.include_paths = include_paths
        _setup_clang()
        self.index = clang.cindex.Index.create()

    def analyze_headers(self, headers):
        result = {
            'functions': {},
            'classes': {},
            'namespaces': set()
        }

        for header in headers:
            try:
                self._analyze_single_header(header, result)
            except Exception as e:
                print(f"Warning: Could not parse {header}: {e}")

        return result

    def _analyze_single_header(self, header, result):
        args = self._get_compile_args()

        header_path = None

        for include_path in self.include_paths:
            potential_path = os.path.join(include_path, header)
            if os.path.exists(potential_path):
                header_path = potential_path
                break

        tu = self.index.parse(header_path, args=args)

        if tu.diagnostics:
            for diag in tu.diagnostics:
                if diag.severity >= clang.cindex.Diagnostic.Error:
                    print(f"Error in {header}: {diag.spelling}")

        self._extract_from_cursor(tu.cursor, result)

    def _get_compile_args(self):
        args = ['-x', 'c++', '-std=c++17']

        if os.name == "nt":
            pass
        else:
            args.extend([
                '-I/usr/include',
                '-I/usr/local/include',
            ])

        for path in self.include_paths:
            if os.name == "nt":
                args.append(f"-I{os.path.abspath(path)}")
            else:
                args.append(f"-I{Path(path).absolute().as_posix()}")

        return args

    def _extract_from_cursor(self, cursor, result):
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.NAMESPACE:
                result["namespaces"].add(child.spelling)
                self._extract_from_cursor(child, result)

            elif child.kind == clang.cindex.CursorKind.FUNCTION_DECL:
                func_info = _extract_function_info(child)
                if func_info and _is_public_function(child):
                    result["functions"][func_info["name"]] = func_info

            elif child.kind in [clang.cindex.CursorKind.CLASS_DECL, clang.cindex.CursorKind.STRUCT_DECL]:
                class_info = _extract_class_info(child)
                if class_info:
                    result["classes"][class_info["name"]] = class_info

            elif child.kind == clang.cindex.CursorKind.CLASS_TEMPLATE:
                self._extract_from_cursor(child, result)
