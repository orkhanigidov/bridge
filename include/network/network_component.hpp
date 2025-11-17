/**
 * Project: Engine
 * File: network_component.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file network_component.hpp
 * @brief Declares the NetworkComponent class providing Oat++ DI components for networking.
 */

#pragma once

#include <memory>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Address.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "server/server_config.hpp"

namespace engine::network
{
    /**
     * @class NetworkComponent
     * @brief Provides Oat++ dependency injection components for networking.
     */
    class NetworkComponent final
    {
    public:
        /**
         * @brief Default constructor.
         */
        NetworkComponent() = default;

        /**
         * @brief Provides a shared ServerConnectionProvider component.
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider)([]
        {
            OATPP_COMPONENT(std::shared_ptr<server::ServerConfig>, config);
            return oatpp::network::tcp::server::ConnectionProvider::createShared({
                config->host, config->port, oatpp::network::Address::IP_4
            });
        }());

        /**
         * @brief Provides a shared HttpRouter component.
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, http_router)([]
        {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        /**
         * @brief Provides a shared ConnectionHandler component.
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connection_handler)([]
        {
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
            return oatpp::web::server::HttpConnectionHandler::createShared(router);
        }());

        /**
         * @brief Provides a shared JSON ObjectMapper component with beautifier enabled.
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper)([]
        {
            auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
            mapper->getSerializer()->getConfig()->useBeautifier = true;
            return mapper;
        }());
    };
} // namespace engine::network
