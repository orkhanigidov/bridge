"""
Script to generate C++ registry implementation from TOML configuration.
"""

import sys
import tomllib
from pathlib import Path
from typing import Dict, Set, Any, Optional


def read_config(config_path: Path) -> Optional[Dict[str, Any]]:
    """Read and parse the TOML configuration file."""
    try:
        with open(config_path, "rb") as f:
            return tomllib.load(f)
    except FileNotFoundError:
        print(f"Error: Configuration file not found at {config_path}", file=sys.stderr)
        return None
    except tomllib.TOMLDecodeError as e:
        print(f"Error parsing TOML: {e}", file=sys.stderr)
        return None


def find_header_file(class_name: str, include_path: str) -> Optional[str]:
    """Find the header file for a given class name in the specified include path."""
    header_name = f"{class_name}.h"
    include_dir = Path(include_path)

    if not include_dir.exists():
        print(f"Warning: Include directory {include_dir} not found.", file=sys.stderr)
        return None

    # Recursively search for the header file through all subdirectories
    matching_files = list(include_dir.glob(f"**/{header_name}"))
    if matching_files:
        rel_path = matching_files[0].relative_to(include_dir)
        return str(rel_path).replace("\\", "/")  # Use forward slashes for cross-platform compatibility

    return None


def extract_headers(config: Dict[str, Any], include_path: str) -> Set[str]:
    """Extract required C++ header files based on the configuration."""
    required_headers = {"memory"}  # Always include memory for std::shared_ptr

    for obj in config.get("api_objects", []):
        cpp_class = obj.get("cpp_class")
        if not cpp_class or "::" not in cpp_class:
            continue

        namespace, class_name = cpp_class.rsplit("::", 1)
        header_path = find_header_file(class_name, include_path)

        if header_path:
            # Ensure header path includes for proper inclusion
            full_path = header_path if header_path.startswith(namespace) else f"{namespace}/{header_path}"
            required_headers.add(full_path)
        else:
            print(f"Warning: Could not find header for {cpp_class}", file=sys.stderr)

    return required_headers


def generate_implementation(config: Dict[str, Any]) -> str:
    """Generate C++ implementation code from configuration."""
    lines = ["// Auto-generated file. Do not edit manually", "#include \"../../include/GeneratedRegistry.h\"", ""]

    # Get include path from configuration
    include_path = config.get("project", {}).get("include_path")
    if not include_path:
        print("Error: No include path found in configuration.", file=sys.stderr)
        return "Error: Please add 'include_dir' to the [project] section."

    # Extract required C++ headers from the configuration
    headers = extract_headers(config, include_path)
    for header in sorted(headers):
        if header == "memory":
            lines.append("#include <memory>")
            lines.append("")
        else:
            lines.append(f"#include <{header}>")

    lines.extend(["", "namespace generated {"])

    # Build object-to-class mapping and declare shared pointers for all objects
    obj_to_class = {}
    for obj in config.get("api_objects", []):
        identifier = obj.get("identifier")
        cpp_class = obj.get("cpp_class")
        if identifier and cpp_class:
            obj_to_class[identifier] = cpp_class
            lines.append(f"\tstd::shared_ptr<{cpp_class}> g_{identifier};")
    lines.append("")

    # Register API objects with std::make_shared
    lines.append("\tvoid registerObjects(Registry *registry) {")
    for obj in config.get("api_objects", []):
        identifier = obj.get("identifier")
        cpp_class = obj.get("cpp_class")
        if identifier and cpp_class:
            lines.append(f"\t\tg_{identifier} = std::make_shared<{cpp_class}>();")
            lines.append(f"\t\tregistry->registerObject(\"{identifier}\", g_{identifier}.get());")
    lines.append("\t}")
    lines.append("")

    # Register all methods
    lines.append("\tvoid registerAllMethods(Registry *registry) {")

    # Global methods
    for method in config.get("bindings", {}).get("global", []):
        api_name = method.get("api_name")

        if api_name:
            cpp_function = method.get("cpp_function", api_name)
            lines.append(f"\t\tregistry->registerGlobalMethod(\"{api_name}\", &ogdf::{cpp_function});")

    if config.get("bindings", {}).get("global", []):
        lines.append("")

    # Member methods
    for method in config.get("bindings", {}).get("member", []):
        object_name = method.get("object")
        api_name = method.get("api_name")

        if object_name and api_name and object_name in obj_to_class:
            cpp_method = method.get("cpp_method", f"{obj_to_class[object_name]}::{api_name}")
            if "overload_signature" in method:
                pass
            else:
                lines.append(f"\t\tregistry->registerMemberMethod(\"{object_name}\", \"{api_name}\", &{cpp_method});")

    lines.append("\t}")
    lines.append("}")

    return "\n".join(lines)


def main() -> None:
    config_dir = Path(__file__).parent.resolve() / "config"
    config_path = config_dir / "registry_config.toml"

    print(config_path, file=sys.stderr)

    config = read_config(config_path)
    if not config:
        sys.exit(1)

    print(generate_implementation(config))


if __name__ == "__main__":
    main()
