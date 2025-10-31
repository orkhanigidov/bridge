/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>

namespace engine::network::server
{
    struct ServerConfig
    {
        oatpp::String host;
        v_uint16 port;
    };

    ServerConfig parse_server_config(const oatpp::base::CommandLineArguments& cmd_args);
} // namespace engine::network::server
