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
    lines = ["// Auto-generated file. Do not edit manually", "#include \"../../include/GeneratedRegistry.h\"", "",
             "namespace generated {", "\tvoid registerObjects(Registry *registry) {"]

    # Register API objects
    for obj in config.get("api_objects", []):
        lines.append(f"\t\tregistry->registerObject(\"{obj['identifier']}\", new {obj['cpp_class']});")
    lines.append("\t}")
    lines.append("")

    # Register all methods
    lines.append("\tvoid registerAllMethods(Registry *registry) {")

    # Global methods
    for method in config.get("bindings", {}).get("global", []):
        lines.append(f"\t\tregistry->registerGlobalMethod(\"{method['api_name']}\", &{method['cpp_function']});")
    lines.append("")

    # Member methods
    for method in config.get("bindings", {}).get("member", []):
        object_name = method["object"]
        api_name = method["api_name"]
        cpp_method = method["cpp_method"]

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
