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
#include <string>

#include "engine.hpp"
#include "io/env_reader.hpp"

namespace
{
    constexpr auto DEFAULT_WRAPPER_DIR = "tools/codegen/include/wrappers";
    constexpr auto DEFAULT_CONFIG_YAML = "bindings_config.yaml";
    constexpr auto DEFAULT_OUTPUT_DIR = "build";

    std::filesystem::path get_path_or_default(const codegen::io::EnvReader& reader,
                                              const std::string& key,
                                              const std::filesystem::path& default_value)
    {
        if (const auto opt_value = reader.get(key); opt_value && !opt_value->empty())
        {
            return *opt_value;
        }

        return default_value;
    }
}

int main(int argc, const char* argv[])
{
    try
    {
        const std::filesystem::path env_path = argc > 1 ? argv[1] : ".env";
        const codegen::io::EnvReader reader(env_path);

        const std::filesystem::path project_root = std::filesystem::absolute(env_path).parent_path();

        const std::filesystem::path include_dir = reader.get("INCLUDE_DIR").value();
        const std::filesystem::path wrapper_dir = get_path_or_default(reader, "WRAPPER_DIR", project_root / DEFAULT_WRAPPER_DIR);
        const std::filesystem::path config_yaml = get_path_or_default(reader, "CONFIG_YAML", project_root / DEFAULT_CONFIG_YAML);
        const std::filesystem::path output_dir = get_path_or_default(reader, "OUTPUT_DIR", project_root / DEFAULT_OUTPUT_DIR);

        const codegen::Engine engine(include_dir, wrapper_dir, config_yaml, output_dir, &std::cout);
        engine.generate_lua_bindings();
    } catch (const std::exception& e)
    {
        std::cerr << std::format("\nFATAL ERROR: {}\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
