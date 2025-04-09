"""
Script to generate C++ registry implementation from TOML configuration.
"""

import sys
import tomllib
from pathlib import Path
from typing import Dict, Any, Optional


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


def generate_implementation(config: Dict[str, Any]) -> str:
    """Generate C++ implementation code from configuration."""
    lines = ["// Auto-generated file. Do not edit manually",
             "#include \"../../include/GeneratedRegistry.h\"",
             "",
             "#include <memory>",
             "",
             "namespace generated {"]

    # Declare shared pointers for all objects
    for obj in config.get("api_objects", []):
        identifier = obj.get("identifier")
        cpp_class = obj.get("cpp_class")
        if identifier and cpp_class:
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
        cpp_function = method.get("cpp_function")
        if api_name and cpp_function:
            lines.append(f"\t\tregistry->registerGlobalMethod(\"{api_name}\", &{cpp_function});")

    if config.get("bindings", {}).get("global", []):
        lines.append("")

    # Member methods
    for method in config.get("bindings", {}).get("member", []):
        object_name = method.get("object")
        api_name = method.get("api_name")
        cpp_method = method.get("cpp_method")

        if object_name and api_name and cpp_method:
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
