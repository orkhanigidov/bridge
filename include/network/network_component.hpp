#pragma once

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

namespace network {
    class NetworkComponent final {
    public:
        explicit NetworkComponent(
                const oatpp::base::CommandLineArguments& cmd_args);

        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                object_mapper)([] {
            auto object_mapper = oatpp::parser::json::mapping::ObjectMapper::
                    createShared();
            object_mapper->getSerializer()->getConfig()->useBeautifier = true;
            return object_mapper;
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                               http_router)([] {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::network::ConnectionHandler>,
                connection_handler)([] {
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                            http_router);
            return oatpp::web::server::HttpConnectionHandler::createShared(
                    http_router);
        }());

        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                server_connection_provider)([this] {
            return oatpp::network::tcp::server::ConnectionProvider::
                    createShared({m_host.c_str(), m_port,
                                  oatpp::network::Address::IP_4});
        }());

    private:
        oatpp::base::CommandLineArguments m_cmd_args;
        std::string m_host{"0.0.0.0"};
        v_uint16 m_port{8000};
    };
} // namespace network
