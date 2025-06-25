#include "network/NetworkManager.hpp"

#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/Deserializer.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace engine::network
{
    NetworkManager::NetworkManager(const oatpp::String& base_url)
    {
        auto connection_provider = oatpp::network::tcp::client::ConnectionProvider::createShared(
            {"localhost", 80, oatpp::network::Address::IP_4});

        auto request_executor =
            oatpp::web::client::HttpRequestExecutor::createShared(connection_provider);

        auto object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

        client_ = NetworkManagerApiClient::createShared(request_executor, object_mapper);

        if (base_url && base_url->size() > 0)
        {
            set_base_url(base_url);
        }
    }

    oatpp::String NetworkManager::get(const oatpp::String& path)
    {
        try
        {
            auto response = client_->get(path);

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

    oatpp::String NetworkManager::post(const oatpp::String& path, const oatpp::String& body)
    {
        try
        {
            auto response = client_->post(path, body);

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

    oatpp::String NetworkManager::put(const oatpp::String& path, const oatpp::String& body)
    {
        try
        {
            auto response = client_->put(path, body);

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

    oatpp::String NetworkManager::del(const oatpp::String& path)
    {
        try
        {
            auto response = client_->del(path);

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

    bool NetworkManager::is_connected()
    {
        try
        {
            auto response = client_->get("get");
            return response->getStatusCode() == 200;
        }
        catch (const std::exception& e)
        {
            return false;
        }
    }

    void NetworkManager::set_base_url(const oatpp::String& base_url)
    {
        OATPP_LOGI("NetworkManager", "Base URL set to: %s", base_url->c_str());
    }
} // namespace engine::network