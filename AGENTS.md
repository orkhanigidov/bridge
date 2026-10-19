# Repository Guidelines

## Project structure

- `src/` contains engine and service implementations; public C++ interfaces belong in `include/`.
- `tools/codegen/` generates Lua bindings from the configured API. Treat generated output and `build/` as build artifacts; change the generator or its inputs instead.
- `tests/` contains Python integration, stress, and benchmark scripts. `examples/` contains Lua scripts and graph fixtures.
- Read the relevant sections of `README.md` before changing build setup, execution modes, or the client/server interface.

## C++ changes

- Keep the project’s C++17 standard and follow the surrounding naming, formatting, and header/source organization.
- Prefer clear ownership and RAII for resources. Preserve existing thread-safety and Lua-state isolation guarantees.
- Treat Lua scripts and network requests as untrusted input. Preserve the sandbox whitelist, execution limits, and validation when changing script execution.
- Keep API and DTO changes consistent across the C API and network interface where applicable.
- Avoid unrelated refactors; update focused tests or examples when behavior changes.

## Build and verification

- Configure and build with the checked-in CMake presets:
  - `cmake --preset Debug`
  - `cmake --build --preset Debug`
- Repeat with `Release` when a change is configuration- or optimization-sensitive.
- Code generation reads local `.env` configuration. If needed, create it from `.env.example` and set the required local paths; never commit `.env` or credentials.
- The Python scripts in `tests/` exercise a running server and may need their documented fixtures and Python dependencies. Run the relevant script only when its prerequisites are available; do not mistake these integration checks for standalone unit tests.

## Change hygiene

- Do not edit vendored dependencies under `third_party/` or downloaded dependencies under `build/_deps/` to implement project behavior.
- Keep changes focused and preserve existing interfaces unless the task explicitly calls for a breaking change.
- Report the build or test commands run and any environment-dependent checks that could not be run.
