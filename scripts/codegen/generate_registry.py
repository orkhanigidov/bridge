import argparse
import logging
import sys
from pathlib import Path
from typing import Dict, Any

import yaml
from jinja2 import Environment, FileSystemLoader, StrictUndefined

logger = logging.getLogger(__name__)


class ConfigError(Exception):
    """ Raised when configuration cannot be loaded or is invalid """


def load_yaml(path: Path) -> Dict[str, Any]:
    # Load and parse a YAML file into a Python dict
    if not path.exists():
        raise ConfigError(f"Config file not found: {path}")

    try:
        data = yaml.safe_load(path.read_text(encoding="utf-8"))
    except yaml.YAMLError as e:
        raise ConfigError(f"Failed to parse YAML '{path}': {e}") from e

    logger.info("Loaded config from %s", path)
    return data or {}


def render_registry(config: Dict[str, Any], templates_dir: Path) -> str:
    # Render the Jinja2 template named 'reflection_registration_template.j2' using the provided config
    if not templates_dir.exists():
        raise ConfigError(f"Templates directory not found: {templates_dir}")

    env = Environment(
        loader=FileSystemLoader(str(templates_dir)),
        undefined=StrictUndefined,
        trim_blocks=True,
        lstrip_blocks=True
    )

    template = env.get_template("reflection_registration_template.j2")
    return template.render(config=config)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generate C++ Registry source from YAML config + Jinja2 template"
    )
    parser.add_argument(
        "--config",
        type=Path,
        default=Path(__file__).resolve().parent / "config" / "reflection_config.yaml",
        help="Path to reflection_config.yaml"
    )
    parser.add_argument(
        "--templates",
        type=Path,
        default=Path(__file__).resolve().parent / "templates",
        help="Path to Jinja2 template directory"
    )

    args = parser.parse_args()

    try:
        config = load_yaml(args.config)
        output = render_registry(config, args.templates)
    except ConfigError as e:
        logger.error(e)
        sys.exit(1)

    sys.stdout.write(output)


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s %(levelname)-8s %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S"
    )
    main()
