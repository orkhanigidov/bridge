#include "engine/core/engine.hpp"
#include "engine/core/engine_mode.hpp"
#include "engine/lua_bindings/lua_registrar.hpp"

namespace
{
    constexpr auto DEFAULT_HOST = "localhost";
    constexpr int DEFAULT_PORT  = 8080;
    constexpr auto DEFAULT_MODE = engine::core::engine_mode::Native;

    void print_usage(const char* prog_name)
    {
        std::cout << "Usage: " << prog_name << " [mode] [host] [port]\n"
                  << " mode: native | network (default: native)\n"
                  << " host: hostname or IP address (default: " << DEFAULT_HOST << ")\n"
                  << " port: port number (default: " << DEFAULT_PORT << ")\n";
    }

} // namespace

int main(int argc, char* argv[])
{
    std::string host = DEFAULT_HOST;
    int port         = DEFAULT_PORT;
    auto mode        = DEFAULT_MODE;

    try
    {
        // if (argc > 1)
        // {
        //     mode = engine::execution::parse(argv[1]);
        // }
        //
        // if (mode == engine::core::engine_mode::Network)
        // {
        //     if (argc > 2)
        //     {
        //         host = argv[2];
        //     }
        //
        //     if (argc > 3)
        //     {
        //         port = std::stoi(argv[3]);
        //         if (port <= 0 || port > 65535)
        //         {
        //             throw std::out_of_range("Port must be between 1 and 65535");
        //         }
        //     }
        // }
        //
        // switch (mode)
        // {
        //     case engine::core::engine_mode::Native:
        //         break;
        //     case engine::core::engine_mode::Network:
        //         break;
        //     default:
        //         std::cerr << "Unknown execution mode\n";
        //         print_usage(argv[0]);
        //         return 1;
        // }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}