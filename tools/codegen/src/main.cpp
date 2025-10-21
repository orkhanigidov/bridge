#include "engine.hpp"
#include "io/env_reader.hpp"

int main(int argc, const char* argv[])
{
    try
    {
        const codegen::io::EnvReader reader;

        const auto include_dir = reader.get("INCLUDE_DIR");
        const auto config_yaml = reader.get("CONFIG_YAML");

        if (!include_dir || !config_yaml)
        {
            throw std::runtime_error("INCLUDE_DIR and CONFIG_YAML environment variables are required.");
        }
        const codegen::io::EnvReader reader(argc > 1 ? argv[1] : ".env");

        const fs::path include_dir = reader.get("INCLUDE_DIR").value();
        const fs::path wrapper_dir = reader.get("WRAPPER_DIR").value();
        const fs::path config_yaml = reader.get("CONFIG_YAML").value();
        const fs::path output_dir = reader.get("OUTPUT_DIR").value();

        const codegen::Engine engine(include_dir, wrapper_dir, config_yaml, output_dir, &std::cout);
        engine.generate_lua_bindings();
    } catch (const std::exception& e)
    {
        std::cerr << std::format("\nFATAL ERROR: {}\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
