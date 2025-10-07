#include "network/network_component.hpp"
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
            try
            {
                server_manager->shutdown();
            }
            catch (const std::exception& e)
            {
                OATPP_LOGE("Main", "Error during server shutdown: %s", e.what());
            }
        }
    }
}

int main(int argc, const char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    oatpp::base::Environment::init();

    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    oatpp::base::CommandLineArguments cmd_args(argc, argv);

    try
    {
        server_manager = std::make_shared<engine::network::ServerManager>(cmd_args);
        server_manager->initialize();
        server_manager->start();
    }
    catch (const std::exception& e)
    {
        OATPP_LOGE("Main", "Server startup failed: %s", e.what());
        exit_code = EXIT_FAILURE;
    } catch (...)
    {
        OATPP_LOGE("Main", "Server startup failed: unknown exception");
        exit_code = EXIT_FAILURE;
    }

    oatpp::base::Environment::destroy();
    OATPP_LOGI("Main", "Application stopped. Exiting with code %d", exit_code);

    return exit_code;
}
