#include "engine.hpp"

int main(int argc, const char* argv[])
{
    codegen::Engine engine;
    engine.generate_lua_bindings();
    return EXIT_SUCCESS;
}
