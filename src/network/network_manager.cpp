#include "engine/network/network_manager.hpp"

#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

namespace engine::network
{
    network_manager::network_manager(const oatpp::String& host, v_uint16 port)
    {
        const auto connection_provider =
            oatpp::network::tcp::client::ConnectionProvider::createShared(
                {host, port, oatpp::network::Address::IP_4});

        const auto request_executor =
            oatpp::web::client::HttpRequestExecutor::createShared(connection_provider);

        const auto object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        api_client_ = api_client::createShared(request_executor, object_mapper);
    }

    oatpp::String network_manager::get(const oatpp::String& path) const
    {
        try
        {
            const auto response = api_client_->get(path);

            if (response->getStatusCode() == 200)
            {
                return response->readBodyToString();
            }

            OATPP_LOGE("NetworkManager", "GET request failed with status: %d",
                       response->getStatusCode());
            return nullptr;
        }
        catch (const std::exception& e)
        {
            OATPP_LOGE("NetworkManager", "GET request exception: %s", e.what());
            return nullptr;
        }
    }

    oatpp::String network_manager::post(const oatpp::String& path, const oatpp::String& body) const
    {
        try
        {
            const auto response = api_client_->post(path, body);

            if (response->getStatusCode() == 200 || response->getStatusCode() == 201)
            {
                return response->readBodyToString();
            }

            OATPP_LOGE("NetworkManager", "POST request failed with status: %d",
                       response->getStatusCode());
            return nullptr;
        }
        catch (const std::exception& e)
        {
            OATPP_LOGE("NetworkManager", "POST request exception: %s", e.what());
            return nullptr;
        }
    }

    oatpp::String network_manager::put(const oatpp::String& path, const oatpp::String& body) const
    {
        try
        {
            const auto response = api_client_->put(path, body);

            if (response->getStatusCode() == 200)
            {
                return response->readBodyToString();
            }

            OATPP_LOGE("NetworkManager", "PUT request failed with status: %d",
                       response->getStatusCode());
            return nullptr;
        }
        catch (const std::exception& e)
        {
            OATPP_LOGE("NetworkManager", "PUT request exception: %s", e.what());
            return nullptr;
        }
    }

    oatpp::String network_manager::del(const oatpp::String& path) const
    {
        try
        {
            const auto response = api_client_->del(path);

            if (response->getStatusCode() == 200 || response->getStatusCode() == 204)
            {
                return response->readBodyToString();
            }

            OATPP_LOGE("NetworkManager", "DELETE request failed with status: %d",
                       response->getStatusCode());
            return nullptr;
        }
        catch (const std::exception& e)
        {
            OATPP_LOGE("NetworkManager", "DELETE request exception: %s", e.what());
            return nullptr;
        }
    }

    bool network_manager::is_connected() const
    {
        try
        {
            const auto response = api_client_->get("health");
            return response && response->getStatusCode() == 200;
        }
        catch (...)
        {
            return false;
        }
    }
} // namespace engine::network