/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file server_manager.hpp
 * @brief Declares the ServerManager class for managing the server lifecycle.
 */

#pragma once

#include <memory>
#include <utility>
#include <oatpp/network/Server.hpp>

#include "server_config.hpp"
#include "server_state.hpp"
#include "network/network_component.hpp"

namespace engine::network::server
{
    /**
     * @class ServerManager
     * @brief Manages the initialization, running, and shutdown of the server.
     */
    class ServerManager final
    {
    public:
        /**
         * @brief Constructs a ServerManager with the given configuration.
         * @param config The server configuration.
         */
        explicit ServerManager(ServerConfig config) : config_(std::move(config))
        {
        }

        /**
         * @brief Destructor the ServerManager.
         */
        ~ServerManager() noexcept;

        /**
         * @brief Runs the server.
         */
        void run();

        /**
         * @brief Shuts down the server.
         */
        void shutdown();

    private:
        /**
         * @brief The server configuration.
         */
        ServerConfig config_;

        /**
         * @brief The current server state.
         */
        ServerState state_{ServerState::Stopped};

        /**
         * @brief The network component.
         */
        std::unique_ptr<NetworkComponent> network_component_;

        /**
         * @brief The Oat++ server instance.
         */
        std::unique_ptr<oatpp::network::Server> server_;

        /**
         * @brief Initializes the server components.
         */
        void initialize();

        /**
         * @brief Starts the server.
         */
        void start();
    };
} // namespace engine::network::server
