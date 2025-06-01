import argparse
import os
import sys
from pathlib import Path

from src.reflection_generator import ReflectionGenerator

sys.path.append(str(Path(__file__).parent / "src"))


def _get_dependency_include_paths(package_names=None):
    try:
        project_root = Path(__file__).parent.parent.parent
        build_dirs = [
            project_root / "build",
            project_root / "cmake-build-debug",
            project_root / "cmake-build-release"
        ]

        include_paths = []

        for build_dir in build_dirs:
            if not build_dir.exists():
                continue

            deps_dir = build_dir / "_deps"

            if not deps_dir.exists():
                continue

            for pkg_dir in deps_dir.glob("*-src"):
                include_dir = pkg_dir / "include"

                if not include_dir.exists():
                    continue

                pkg_name = pkg_dir.name.replace("-src", "")

                if not package_names:
                    if os.name == "nt":
                        include_paths.append(str(include_dir))
                    else:
                        include_paths.append(include_dir.as_posix())
                    continue

                if any(package.lower() in pkg_name.lower() for package in package_names):
                    if os.name == "nt":
                        include_paths.append(str(include_dir))
                    else:
                        include_paths.append(include_dir.as_posix())

        return include_paths

    except Exception as e:
        print(f"Warning: Failed to scan dependencies: {e}", file=sys.stderr)
        return []


def main():
    parser = argparse.ArgumentParser(description="Generate reflection registration from config")
    parser.add_argument("--config", required=True, help="YAML config file")
    parser.add_argument("--include", action="append", help="Include paths", default=[])
    parser.add_argument("--package", action="append", help="Package names to include", default=[])
    parser.add_argument("--output", help="Output file (default: print to stdout)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")

    args = parser.parse_args()

    include_paths = args.include

    if not include_paths:
        include_paths = _get_dependency_include_paths(args.package)
        if args.verbose and include_paths:
            pkg_info = f" for packages {args.package}" if args.package else ""
            print(f"Using dependency include paths{pkg_info}: {include_paths}")
    elif args.verbose:
        print(f"Using user-specified include paths: {include_paths}")

    if args.verbose:
        print(f"Config: {args.config}")
        print(f"Output: {args.output}")

    try:
        config_path = Path(args.config)

        if not config_path.exists():
            raise FileNotFoundError(f"Config file not found: {args.config}")

        generator = ReflectionGenerator(include_paths)
        code = generator.generate_from_config(str(config_path), args.output)

        if not args.output:
            print(code)

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
