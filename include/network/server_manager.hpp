#pragma once

#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "execution_controller.hpp"
#include "network_component.hpp"

namespace network {

    class ServerManager final {
    public:
        explicit ServerManager(const oatpp::base::CommandLineArguments& cmd_args):
            m_cmd_args(cmd_args) {}

        ~ServerManager() noexcept;

        void initialize();
        void start();
        void shutdown();

        bool is_initialized() const noexcept
        {
            return m_is_initialized;
        }

        bool is_running() const noexcept
        {
            return m_is_running;
        }

    private:
        oatpp::base::CommandLineArguments m_cmd_args;
        std::shared_ptr<oatpp::network::Server> m_server;
        bool m_is_initialized{false};
        bool m_is_running{false};
    };

} // namespace network
