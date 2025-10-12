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

        const codegen::Engine engine(include_dir.value(), config_yaml.value());
        engine.generate_lua_bindings();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
