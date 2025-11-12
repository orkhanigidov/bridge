/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file server_config.cpp
 * @brief Implements parsing of server configuration from command-line arguments.
 */

#include "network/server/server_config.hpp"

#include <exception>
#include <stdexcept>
#include <string_view>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>

namespace
{
    constexpr v_int32 MIN_PORT = 1;
    constexpr v_int32 MAX_PORT = 65535;

    constexpr std::string_view HOST_ARG = "--host";
    constexpr std::string_view PORT_ARG = "--port";
    constexpr std::string_view DEFAULT_HOST = "localhost";
    constexpr std::string_view DEFAULT_PORT = "8000";

    /**
     * @brief Checks if the given port is within the valid range.
     * @param port The port number to validate
     * @return True if the port is valid, false otherwise.
     */
    bool is_valid_port(v_int32 port)
    {
        return port >= MIN_PORT && port <= MAX_PORT;
    }
}

namespace engine::network::server
{
    /**
     * @brief Parses server configuration from command-line arguments.
     * @param cmd_args The command-line arguments.
     * @return The parsed ServerConfig.
     * @throws std::invalid_argument If the port value is not a valid integer.
     * @throws std::out_of_range If the port is outside the valid range.
     */
    ServerConfig parse_server_config(const oatpp::base::CommandLineArguments& cmd_args)
    {
        ServerConfig config;

        config.host = cmd_args.getNamedArgumentValue(HOST_ARG.data(), DEFAULT_HOST.data());
        const auto port_str = cmd_args.getNamedArgumentValue(PORT_ARG.data(), DEFAULT_PORT.data());

        v_int32 port_val;
        try
        {
            port_val = oatpp::utils::conversion::strToInt32(port_str);
        } catch (const std::exception& e)
        {
            throw std::invalid_argument("Invalid port number format" + std::string(e.what()));
        }

        if (!is_valid_port(port_val))
        {
            throw std::out_of_range("Invalid port number. Valid range is" + std::to_string(MIN_PORT) + " to " + std::to_string(MAX_PORT) + ".");
        }

        config.port = static_cast<v_uint16>(port_val);

        return config;
    }
} // namespace engine::network::server
