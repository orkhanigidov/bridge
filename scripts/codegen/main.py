from src.reflection_generator import ReflectionGenerator
import argparse
import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent / 'src'))


def main():
    parser = argparse.ArgumentParser(
        description="Generate reflection registration from config")
    parser.add_argument('--config', required=True, help="YAML config file")
    parser.add_argument(
        '--output', help="Output file (default: print to stdout)")
    parser.add_argument('--include', action='append',
                        help="Include paths", default=[])
    parser.add_argument('--verbose', '-v',
                        action='store_true', help="Verbose output")

    args = parser.parse_args()

    if args.verbose:
        print(f"Config: {args.config}")
        print(f"Output: {args.output}")
        print(f"Includes: {args.include}")

    try:
        generator = ReflectionGenerator(include_paths=args.include)
        code = generator.generate_from_config(args.config, args.output)

        if not args.output:
            print(code)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
