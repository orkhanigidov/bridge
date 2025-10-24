#include "network/server/server_manager.hpp"

#include <exception>
#include <memory>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "network/network_component.hpp"
#include "network/api/execution_controller.hpp"
#include "network/server/server_config.hpp"
#include "network/server/server_state.hpp"

namespace engine::network::server
{
    ServerManager::~ServerManager() noexcept
    {
        if (state_ == ServerState::RUNNING || state_ == ServerState::INITIALIZED)
        {
            OATPP_LOGW("Server", "Server is still running. Shutting down...");
            shutdown();
        }
    }

    void ServerManager::initialize()
    {
        if (state_ != ServerState::STOPPED)
        {
            OATPP_LOGW("Server", "Initialization skipped - already initialized");
            return;
        }

        OATPP_LOGI("Server", "Initializing...");

        OATPP_CREATE_COMPONENT(std::shared_ptr<engine::network::server::ServerConfig>, config)([this]
        {
            return std::make_shared<ServerConfig>(config_);
        }());

        network_component_ = std::make_unique<NetworkComponent>();

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper);

        auto execution_controller = std::make_shared<api::ExecutionController>(object_mapper);

        http_router->addController(execution_controller);

        server_ = std::make_unique<oatpp::network::Server>(connection_provider, connection_handler);

        state_ = ServerState::INITIALIZED;
        OATPP_LOGI("Server", "Initialized on %s:%u", config_.host->c_str(), config_.port);
    }

    void ServerManager::run()
    {
        try
        {
            initialize();

            if (state_ == ServerState::INITIALIZED)
            {
                start();
            }
        } catch (const std::exception& e)
        {
            OATPP_LOGE("Server", "Error during server startup: %s", e.what());
            shutdown();
        }
    }

    void ServerManager::start()
    {
        state_ = ServerState::RUNNING;
        OATPP_LOGI("Server", "Server is running on %s:%u", config_.host->c_str(), config_.port);
        OATPP_LOGI("Server", "Press Ctrl+C to stop");

        server_->run(); // Blocking call

        OATPP_LOGI("Server", "Server has stopped listening");
        shutdown();
    }

    void ServerManager::shutdown()
    {
        if (state_ != ServerState::RUNNING && state_ != ServerState::INITIALIZED)
        {
            OATPP_LOGW("Server", "Shutdown skipped - server is not running");
            return;
        }

        state_ = ServerState::SHUTTING_DOWN;
        OATPP_LOGI("Server", "Shutting down...");

        try
        {
            if (server_)
            {
                server_->stop();
            }
        } catch (const std::exception& e)
        {
            OATPP_LOGE("Server", "Error during server shutdown: %s", e.what());
        }

        server_.reset();
        network_component_.reset();

        state_ = ServerState::STOPPED;
        OATPP_LOGI("Server", "Shutdown complete");
    }
} // namespace engine::network::server
