#pragma once

#include "execution_controller.hpp"
#include "network_component.hpp"
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

namespace network {
    class ServerManager final {
    public:
        explicit ServerManager(
                const oatpp::base::CommandLineArguments& cmd_args)
            : m_cmd_args(cmd_args)
        {}
        ~ServerManager();

        void initialize();
        void start();
        void shutdown();

        bool is_initialized() const noexcept { return m_is_initialized; }

        bool is_running() const noexcept { return m_is_running; }

    private:
        std::unique_ptr<NetworkComponent> m_network_component;
        oatpp::base::CommandLineArguments m_cmd_args;
        std::shared_ptr<oatpp::web::server::HttpRouter> m_router;
        std::shared_ptr<oatpp::network::ConnectionHandler> m_connection_handler;
        std::shared_ptr<oatpp::network::ServerConnectionProvider>
                m_connection_provider;
        std::shared_ptr<oatpp::network::Server> m_server;
        std::shared_ptr<ExecutionController> m_execution_controller;

        bool m_is_initialized{false};
        bool m_is_running{false};
    };
} // namespace network
