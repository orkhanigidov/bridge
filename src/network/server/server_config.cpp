#include "network/server/server_config.hpp"

#include <oatpp/core/utils/ConversionUtils.hpp>

namespace
{
    constexpr v_int32 MIN_PORT = 1;
    constexpr v_int32 MAX_PORT = 65535;

    constexpr std::string_view HOST_ARG = "--host";
    constexpr std::string_view PORT_ARG = "--port";
    constexpr std::string_view DEFAULT_HOST = "localhost";
    constexpr std::string_view DEFAULT_PORT = "8000";

    bool is_valid_port(v_int32 port)
    {
        return port >= MIN_PORT && port <= MAX_PORT;
    }
}

namespace engine::network::server
{
    ServerConfig parse_server_config(const oatpp::base::CommandLineArguments& cmd_args)
    {
        ServerConfig config;

        config.host = cmd_args.getNamedArgumentValue(HOST_ARG.data(), DEFAULT_HOST.data());
        const auto port_str = cmd_args.getNamedArgumentValue(PORT_ARG.data(), DEFAULT_PORT.data());

        v_int32 port_val;
        try
        {
            port_val = oatpp::utils::conversion::strToInt32(port_str);
        }
        catch (const std::exception& e)
        {
            throw std::invalid_argument(std::format("Invalid port number format: {}", e.what()));
        }

        if (!is_valid_port(port_val))
        {
            throw std::out_of_range(std::format("Invalid port number. Valid range is [{} - {}].", MIN_PORT, MAX_PORT));
        }

        config.port = static_cast<v_uint16>(port_val);

        return config;
    }
} // namespace engine::network::server
