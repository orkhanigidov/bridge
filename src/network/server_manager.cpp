#include "network/server_manager.hpp"

namespace network {

    ServerManager::~ServerManager() noexcept
    {
        try {
            if (m_is_running) {
                shutdown();
            }
        } catch (const std::exception& e) {
            OATPP_LOGE("Server", "Shutdown failed: %s", e.what());
        } catch (...) {
            OATPP_LOGE("Server", "Shutdown failed: unknown exception");
        }
    }

    void ServerManager::initialize()
    {
        if (m_is_initialized) {
            OATPP_LOGW("Server", "Initialization skipped - server already initialized");
            return;
        }

        auto network_component = std::make_unique<NetworkComponent>(m_cmd_args);

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);

        auto execution_controller = std::make_shared<ExecutionController>();
        http_router->addController(execution_controller);

        m_server = std::make_shared<oatpp::network::Server>(connection_provider, connection_handler);

        m_is_initialized = true;
        OATPP_LOGI("Server", "Initialized on %s:%s", connection_provider->getProperty("host").toString(), connection_provider->getProperty("port").toString());
    }

    void ServerManager::start()
    {
        if (!m_is_initialized) {
            OATPP_LOGE("Server", "Start skipped - server not initialized");
            return;
        }

        if (m_is_running) {
            OATPP_LOGW("Server", "Start skipped - server already running");
            return;
        }

        m_is_running = true;
        OATPP_LOGI("Server", "Starting...");

        m_server->run();
        OATPP_LOGI("Server", "Running");

        m_is_running = false;
        OATPP_LOGI("Server", "Stopped");
    }

    void ServerManager::shutdown()
    {
        if (!m_is_initialized) {
            OATPP_LOGW("Server", "Shutdown skipped - server not initialized");
            return;
        }

        if (m_is_running) {
            OATPP_LOGI("Server", "Shutting down...");
            m_server->stop();
            m_is_running = false;
            OATPP_LOGI("Server", "Shutdown complete");
        } else {
            OATPP_LOGW("Server", "Shutdown skipped - server not running");
        }
    }

} // namespace network
