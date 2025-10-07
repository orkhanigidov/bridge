#include "network/server_manager.hpp"

#include "network/controller/execution_controller.hpp"

namespace engine::network
{
    ServerManager::~ServerManager() noexcept
    {
        try
        {
            if (m_is_running)
            {
                shutdown();
            }
        }
        catch (const std::exception& e)
        {
            OATPP_LOGE("Server", "Failed to shutdown: %s", e.what());
        } catch (...)
        {
            OATPP_LOGE("Server", "Failed to shutdown: unknown exception");
        }
    }

    void ServerManager::initialize()
    {
        if (m_is_initialized)
        {
            OATPP_LOGW("Server", "Initialization skipped - already initialized");
            return;
        }

        auto network_component = std::make_unique<NetworkComponent>(m_cmd_args);

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);

        auto execution_controller = std::make_shared<controller::ExecutionController>();
        http_router->addController(execution_controller);

        m_server = std::make_shared<oatpp::network::Server>(connection_provider, connection_handler);

        m_is_initialized = true;
        OATPP_LOGI("Server", "Initialized on %s:%s",
                   connection_provider->getProperty("host").getData(),
                   connection_provider->getProperty("port").getData());
    }

    void ServerManager::start()
    {
        if (!m_is_initialized)
        {
            OATPP_LOGE("Server", "Start skipped - not initialized");
            return;
        }

        if (m_is_running)
        {
            OATPP_LOGW("Server", "Start skipped - already running");
            return;
        }

        m_is_running = true;

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);
        OATPP_LOGI("Server", "Starting on %s:%s",
                   connection_provider->getProperty("host").getData(),
                   connection_provider->getProperty("port").getData());

        m_server->run(); // Blocking call

        m_is_running = false;
        OATPP_LOGI("Server", "Stopped");
    }

    void ServerManager::shutdown()
    {
        if (!m_is_initialized)
        {
            OATPP_LOGW("Server", "Shutdown skipped - not initialized");
            return;
        }

        if (m_is_running)
        {
            OATPP_LOGI("Server", "Shutting down...");
            m_server->stop();
            m_is_running = false;
            OATPP_LOGI("Server", "Shutdown complete");
        }
        else
        {
            OATPP_LOGW("Server", "Shutdown skipped - not running");
        }
    }
} // namespace engine::network
