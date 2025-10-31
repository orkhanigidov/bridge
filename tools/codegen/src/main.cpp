/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <iostream>

#include "engine.hpp"
#include "io/env_reader.hpp"

int main(int argc, const char* argv[])
{
    try
    {
        const codegen::io::EnvReader reader(argc > 1 ? argv[1] : ".env");

        const std::filesystem::path include_dir = reader.get("INCLUDE_DIR").value();
        const std::filesystem::path wrapper_dir = reader.get("WRAPPER_DIR").value();
        const std::filesystem::path config_yaml = reader.get("CONFIG_YAML").value();
        const std::filesystem::path output_dir = reader.get("OUTPUT_DIR").value();

        const codegen::Engine engine(include_dir, wrapper_dir, config_yaml, output_dir, &std::cout);
        engine.generate_lua_bindings();
    } catch (const std::exception& e)
    {
        std::cerr << std::format("\nFATAL ERROR: {}\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
