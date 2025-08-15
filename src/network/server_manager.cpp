#include "network/server_manager.hpp"

namespace network {
    ServerManager::~ServerManager()
    {
        if (m_is_running) { shutdown(); }
    }

    void ServerManager::initialize()
    {
        if (m_is_initialized) {
            OATPP_LOGW("Server", "Server is already initialized");
            return;
        }

        m_network_component = std::make_unique<NetworkComponent>(m_cmd_args);

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                        router);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                        connection_handler);
        OATPP_COMPONENT(
                std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                connection_provider);

        m_router = std::move(router);
        m_connection_handler = std::move(connection_handler);
        m_connection_provider = std::move(connection_provider);

        m_execution_controller = std::make_shared<ExecutionController>();
        m_router->addController(m_execution_controller);

        m_server = std::make_shared<oatpp::network::Server>(
                m_connection_provider, m_connection_handler);

        m_is_initialized = true;
        OATPP_LOGI("Server", "Server initialized on %s:%d",
                   m_connection_provider->getProperty("host").getData(),
                   m_connection_provider->getProperty("port").getData());
    }

    void ServerManager::start()
    {
        if (!m_is_initialized) {
            OATPP_LOGE("Server",
                       "Server not initialized. Call initialize() first");
            return;
        }

        if (m_is_running) {
            OATPP_LOGW("Server", "Server is already running");
            return;
        }

        OATPP_LOGI("Server", "Server started on %s:%d",
                   m_connection_provider->getProperty("host").getData(),
                   m_connection_provider->getProperty("port").getData());

        m_is_running = true;
        m_server->run();
        m_is_running = false;

        OATPP_LOGI("Server", "Server stopped");
    }

    void ServerManager::shutdown()
    {
        if (!m_is_initialized) {
            OATPP_LOGW("Server", "Server not initialized");
            return;
        }

        if (m_is_running) {
            OATPP_LOGI("Server ", "Shutting down server...");
            m_server->stop();
            m_is_running = false;
        }
        else {
            OATPP_LOGW("Server", "Server is not running");
        }
    }
} // namespace network
