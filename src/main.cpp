#include "network/network_component.hpp"
#include "network/server_config.hpp"
#include "network/server_manager.hpp"

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>

static std::shared_ptr<engine::network::ServerManager> server_manager;

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

int main(int argc, const char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    oatpp::base::Environment::init();

    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    try
    {
        const oatpp::base::CommandLineArguments cmd_args(argc, argv);
        auto server_config = engine::network::parse_server_config(cmd_args);

        server_manager = std::make_shared<engine::network::ServerManager>(server_config);
        server_manager->run();
    }
    catch (const std::exception& e)
    {
        OATPP_LOGE("Main", "Server startup failed: %s", e.what());
        exit_code = EXIT_FAILURE;
    }

    oatpp::base::Environment::destroy();
    OATPP_LOGI("Main", "Application stopped. Exiting with code %d", exit_code);

    return exit_code;
}
