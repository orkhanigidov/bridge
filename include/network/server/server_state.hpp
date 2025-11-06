/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
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
