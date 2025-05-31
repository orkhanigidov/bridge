import clang.cindex
import os
from typing import Dict, List, Any, Optional


class LibClangAnalyzer:
    def __init__(self, include_paths):
        self.index = clang.cindex.Index.create()
        self.include_paths = include_paths or []
        self._setup_clang()

    def _setup_clang(self):
        library_path = os.path.join(os.path.dirname(
            os.path.realpath(__file__)), 'native')
        clang.cindex.conf.set_library_file(library_path)

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
        tu = self.index.parse(header, args=args)

        if tu.diagnostics:
            for diag in tu.diagnostics:
                if diag.severity >= clang.cindex.Diagnostic.Error:
                    print(f"Error in {header}: {diag.spelling}")

        self._extract_from_cursor(tu.cursor, result)

    def _get_compile_args(self):
        return [
            '-std=c++17',
            '-I/usr/include',
            '-I/usr/local/include',
            '-I/usr/include/ogdf'
        ] + [f'-I{path}' for path in self.include_paths]

    def _extract_from_cursor(self, cursor, result):
        for child in cursor.get_children():
            if child.kind == clang.cindex.CursorKind.NAMESPACE:
                result['namespaces'].add(child.spelling)
                self._extract_from_cursor(child, result)

            elif child.kind == clang.cindex.CursorKind.FUNCTION_DECL:
                func_info = self._extract_function_info(child)
                if func_info and self._is_public_function(child):
                    result['functions'][func_info['name']] = func_info

            elif child.kind in [clang.cindex.CursorKind.CLASS_DECL,
                                clang.cindex.CursorKind.STRUCT_DECL]:
                class_info = self._extract_class_info(child)
                if class_info:
                    result['classes'][class_info['name']] = class_info

            elif child.kind in [clang.cindex.CursorKind.CLASS_TEMPLATE,
                                clang.cindex.CursorKind.NAMESPACE]:
                self._extract_from_cursor(child, result)

    def _extract_function_info(self, cursor):
        try:
            params = list(cursor.get_arguments())
            return {
                'name': cursor.spelling,
                'return_type': cursor.result_type.spelling,
                'parameters': [p.spelling or f"param{i}" for i, p in enumerate(params)],
                'parameter_types': [p.type.spelling for p in params],
                'is_static': cursor.storage_class == clang.cindex.StorageClass.STATIC,
                'location': f"{cursor.location.file}:{cursor.location.line}" if cursor.location.file else "unknown"
            }
        except Exception as e:
            print(f"Error extracting function {cursor.spelling}: {e}")
            return None

    def _extract_class_info(self, cursor):
        try:
            methods = {}
            static_methods = {}

            for child in cursor.get_children():
                if child.kind == clang.cindex.CursorKind.CXX_METHOD:
                    method_info = self._extract_function_info(child)
                    if method_info and self._is_public_method(child):
                        if child.is_static_method():
                            static_methods[method_info['name']] = method_info
                        else:
                            methods[method_info['name']] = method_info

            return {
                'name': cursor.spelling,
                'methods': methods,
                'static_methods': static_methods
            }

        except Exception as e:
            print(f"Error extracting class {cursor.spelling}: {e}")
            return None

    def _is_public_function(self, cursor):
        return cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC or cursor.linkage == clang.cindex.LinkageKind.EXTERNAL

    def _is_public_method(self, cursor):
        return cursor.access_specifier == clang.cindex.AccessSpecifier.PUBLIC
