#pragma once

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/Types.hpp>

namespace engine::network::server
{
    struct ServerConfig
    {
        oatpp::String host;
        v_uint16 port;
    };

    ServerConfig parse_server_config(const oatpp::base::CommandLineArguments& cmd_args);
} // namespace engine::network::server
