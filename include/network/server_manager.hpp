#pragma once

#include <oatpp/network/Server.hpp>

#include "network_component.hpp"
#include "server_state.hpp"

namespace engine::network
{
    class ServerManager final
    {
    public:
        explicit ServerManager(const ServerConfig& config) : config_(config)
        {
        }

        ~ServerManager() noexcept;

        void run();
        void shutdown();

    private:
        void initialize();
        void start();

        ServerConfig config_;
        ServerState state_{ServerState::STOPPED};
        std::unique_ptr<NetworkComponent> network_component_;
        std::shared_ptr<oatpp::network::Server> server_;
    };
} // namespace engine::network
