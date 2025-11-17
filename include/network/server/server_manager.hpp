/**
 * Project: Engine
 * File: server_manager.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
