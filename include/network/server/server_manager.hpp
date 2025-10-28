#pragma once

#include <memory>
#include <utility>
#include <oatpp/network/Server.hpp>

#include "server_config.hpp"
#include "server_state.hpp"
#include "network/network_component.hpp"

namespace engine::network::server
{
    class ServerManager final
    {
    public:
        explicit ServerManager(ServerConfig config) : config_(std::move(config))
        {
        }

        ~ServerManager() noexcept;

        void run();
        void shutdown();

    private:
        ServerConfig config_;
        ServerState state_{ServerState::Stopped};
        std::unique_ptr<NetworkComponent> network_component_;
        std::unique_ptr<oatpp::network::Server> server_;

        void initialize();
        void start();
    };
} // namespace engine::network::server
