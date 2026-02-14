/**
 * Project: Engine
 * File: server_state.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file server_state.hpp
 * @brief Defines the ServerState enum representing the server's lifecycle state.
 */

#pragma once

namespace engine::network::server
{
    /**
     * @enum ServerState
     * @brief Represents the various states of the server lifecycle.
     */
    enum class ServerState
    {
        /**
         * @brief The server is stopped.
         */
        Stopped,

        /**
         * @brief The server is initialized but not running.
         */
        Initialized,

        /**
         * @brief The server is currently running.
         */
        Running,

        /**
         * @brief The server is in the process of shutting down.
         */
        ShuttingDown
    };
} // namespace engine::network::server
