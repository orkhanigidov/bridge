/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file main.cpp
 * @brief Entry point for the Engine server application.
 */

#include <csignal>
#include <cstdlib>
#include <exception>
#include <memory>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>

#include "execution/script/lua_state_manager.hpp"
#include "execution/script/script_executor.hpp"
#include "network/network_component.hpp"
#include "network/server/server_config.hpp"
#include "network/server/server_manager.hpp"

static std::shared_ptr<engine::network::server::ServerManager> server_manager;

/**
 * @brief Handles termination signals (SIGINT, SIGTERM) to gracefully shut down the server.
 * @param signal The received signal number.
 */
void handle_signal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        if (server_manager)
        {
            OATPP_LOGI("Main", "Signal %d received. Shutting down the server...", signal);
            server_manager->shutdown();
        }
    }
}

/**
 * @brief Main entry point for the Engine application. Initializes the environment, parses configuration, starts the server, and handles shutdown.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit code.
 */
int main(int argc, const char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    oatpp::base::Environment::init();

    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    try
    {
        engine::execution::script::LuaStateManager::get_state();

        const oatpp::base::CommandLineArguments cmd_args(argc, argv);
        auto server_config = engine::network::server::parse_server_config(cmd_args);

        server_manager = std::make_shared<engine::network::server::ServerManager>(server_config);
        server_manager->run();
    } catch (const std::exception& e)
    {
        OATPP_LOGE("Main", "Server startup failed: %s", e.what());
        exit_code = EXIT_FAILURE;
    }

    oatpp::base::Environment::destroy();
    OATPP_LOGI("Main", "Application stopped. Exiting with code %d", exit_code);

    return exit_code;
}
