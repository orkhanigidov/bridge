/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file server_config.hpp
 * @brief Declares the ServerConfig struct and configuration parsing function.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>

namespace engine::network::server
{
    /**
     * @struct ServerConfig
     * @brief Holds the server's host and port configuration.
     */
    struct ServerConfig
    {
        /**
         * @brief The server host address.
         */
        oatpp::String host;

        /**
         * @brief The server port number.
         */
        v_uint16 port;
    };

    /**
     * @brief Parses server configuration from command-line arguments.
     * @param cmd_args The command-line arguments.
     * @return The parsed ServerConfig.
     */
    ServerConfig parse_server_config(const oatpp::base::CommandLineArguments& cmd_args);
} // namespace engine::network::server
