#pragma once

#include <oatpp/network/Server.hpp>

#include "network_component.hpp"
#include "server_state.hpp"

namespace engine::network
{
    class ServerManager final
    {
    public:
        explicit ServerManager(ServerConfig  config) : config_(std::move(config))
        {
        }

        ~ServerManager() noexcept;

        void run();
        void shutdown();

    private:
        ServerConfig config_;
        ServerState state_{ServerState::STOPPED};
        std::unique_ptr<NetworkComponent> network_component_;
        std::unique_ptr<oatpp::network::Server> server_;

        void initialize();
        void start();
    };
} // namespace engine::network
