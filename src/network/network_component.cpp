#include "network/network_component.hpp"

namespace network {
    static constexpr int kMinPort = 1;
    static constexpr int kMaxPort = 65535;

    static bool is_valid_port(v_uint16 port)
    {
        return port >= kMinPort && port <= kMaxPort;
    }

    NetworkComponent::NetworkComponent(
            const oatpp::base::CommandLineArguments& cmd_args)
        : m_cmd_args(cmd_args)
    {
        try {
            const auto host_arg
                    = m_cmd_args.getNamedArgumentValue("--host", "0.0.0.0");
            m_host = host_arg ? host_arg : std::string{"0.0.0.0"};

            const v_uint16 port = oatpp::utils::conversion::strToInt32(
                    m_cmd_args.getNamedArgumentValue("--port", "8000"));
            if (!is_valid_port(port)) {
                throw std::out_of_range("Port number must be in the range "
                                        + std::to_string(kMinPort) + " - "
                                        + std::to_string(kMaxPort));
            }
            m_port = static_cast<v_uint16>(port);
        }
        catch (const std::exception& e) {
            throw std::invalid_argument("Invalid port number format: "
                                        + std::string(e.what()));
        }
    }
} // namespace network
