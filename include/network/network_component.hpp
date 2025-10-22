#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "server/server_config.hpp"

namespace engine::network
{
    class NetworkComponent final
    {
    public:
        NetworkComponent() = default;

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider)([]
        {
            OATPP_COMPONENT(std::shared_ptr<server::ServerConfig>, config);
            return oatpp::network::tcp::server::ConnectionProvider::createShared({
                config->host, config->port, oatpp::network::Address::IP_4
            });
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router)([]
        {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler)([]
        {
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
            return oatpp::web::server::HttpConnectionHandler::createShared(router);
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper)([]
        {
            auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
            mapper->getSerializer()->getConfig()->useBeautifier = true;
            return mapper;
        }());
    };
} // namespace engine::network
