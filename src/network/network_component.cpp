#include "network/network_component.hpp"

namespace {
    
    constexpr int32_t min_port = 1;
    constexpr int32_t max_port = 65535;

    bool is_valid_port(v_int32 port) {
        return port >= min_port && port <= max_port;
    }

}

namespace network {

    NetworkComponent::NetworkComponent(const oatpp::base::CommandLineArguments& cmd_args): m_cmd_args(cmd_args)
    {
        try {
            m_host = m_cmd_args.getNamedArgumentValue("--host", "localhost");

            const auto port_arg = m_cmd_args.getNamedArgumentValue("--port", "8000");
            const v_int32 port = oatpp::utils::conversion::strToInt32(port_arg);
            
            if (!is_valid_port(port)) {
                throw std::out_of_range(std::format("Invalid port number. Valid range is [{} - {}].", min_port, max_port));
            }
            m_port = static_cast<v_uint16>(port);
        }
        catch (const std::exception& e) {
            throw std::invalid_argument(std::format("Invalid port number format: {}", e.what()));
        }
    }

} // namespace network
