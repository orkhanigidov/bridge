/**
 * Project: Engine
 * File: server_config.hpp
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
