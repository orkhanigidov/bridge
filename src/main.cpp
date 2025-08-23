#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>

#include "network/server_manager.hpp"

int main(int argc, const char* argv[]) {

    int exit_code = EXIT_SUCCESS;

    oatpp::base::Environment::init();
    oatpp::base::CommandLineArguments cmd_args(argc, argv);

    try {
        network::ServerManager server_manager(cmd_args);
        server_manager.initialize();
        server_manager.start();
    } catch (const std::exception& e) {
        OATPP_LOGE("Main", "Server startup failed: %s", e.what());
        exit_code = EXIT_FAILURE;
    } catch (...) {
        OATPP_LOGE("Main", "Server startup failed: unknown exception");
        exit_code = EXIT_FAILURE;
    }

    oatpp::base::Environment::destroy();
    return exit_code;
}
