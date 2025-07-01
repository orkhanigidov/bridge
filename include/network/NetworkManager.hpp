#pragma once

#include "HttpClient.hpp"

namespace engine::network
{
    class NetworkManager
    {
      public:
        explicit NetworkManager(const oatpp::String& host, v_uint16 port);
        ~NetworkManager() = default;

        oatpp::String get(const oatpp::String& path) const;
        oatpp::String post(const oatpp::String& path, const oatpp::String& body) const;
        oatpp::String put(const oatpp::String& path, const oatpp::String& body) const;
        oatpp::String del(const oatpp::String& path) const;

        bool is_connected() const;

      private:
        std::shared_ptr<HttpClient> http_client_;
    };
} // namespace engine::network