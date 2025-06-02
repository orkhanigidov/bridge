import argparse
import sys
from pathlib import Path
from typing import Optional, List

from src.reflection_generator import ReflectionGenerator

sys.path.append(str(Path(__file__).parent / "src"))


class DependencyScanner:
    def __init__(self, project_root: Optional[Path] = None) -> None:
        self.project_root = project_root or Path(__file__).parent.parent.parent

    def get_include_paths(self, package_names: Optional[List[str]] = None) -> List[str]:
        try:
            build_dirs = [
                self.project_root / "build",
                self.project_root / "cmake-build-debug",
                self.project_root / "cmake-build-release"
            ]

            include_paths = []

            for build_dir in build_dirs:
                if not build_dir.exists():
                    continue

                deps_dir = build_dir / "_deps"
                if not deps_dir.exists():
                    continue

                include_paths.extend(self._scan_deps_directory(deps_dir, package_names))

            return include_paths

        except Exception as e:
            print(f"Warning: Failed to scan dependencies: {e}", file=sys.stderr)
            return []

    def _scan_deps_directory(self, deps_dir: Path, package_names: Optional[List[str]]) -> List[str]:
        include_paths = []

        for pkg_dir in deps_dir.glob("*-src"):
            include_dir = pkg_dir / "include"
            if not include_dir.exists():
                continue

            pkg_name = pkg_dir.name.replace("-src", "")

            if not package_names or self._matches_package_filter(pkg_name, package_names):
                include_paths.append(str(include_dir.resolve()))

        return include_paths

    def _matches_package_filter(self, pkg_name: str, package_names: List[str]) -> bool:
        return any(package.lower() in pkg_name.lower() for package in package_names)


def create_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Generate reflection registration code from config")
    parser.add_argument("--config", required=True, help="YAML config file")
    parser.add_argument("--include", action="append", help="Include paths", default=[])
    parser.add_argument("--package", action="append", help="Package names to include", default=[])
    parser.add_argument("--output", help="Output file (default: print to stdout)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose output")

    return parser


def main():
    parser = create_argument_parser()
    args = parser.parse_args()

    try:
        config_path = Path(args.config)
        if not config_path.exists():
            raise FileNotFoundError(f"Config file not found: {args.package}")

        include_paths = args.include
        if not include_paths:
            scanner = DependencyScanner()
            include_paths = scanner.get_include_paths(args.package)

            if args.verbose and include_paths:
                pkg_info = f" for packages {args.package}" if args.package else ""
                print(f"Using dependency include paths{pkg_info}: {include_paths}")
        elif args.verbose:
            print(f"Using user-specified include paths: {include_paths}")

        if args.verbose:
            print(f"Config: {args.config}")
            print(f'Output: {args.output or "stdout"}')

        generator = ReflectionGenerator(include_paths)
        code = generator.generate_from_config(str(config_path), args.output)

        if not args.output:
            print(code)

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
