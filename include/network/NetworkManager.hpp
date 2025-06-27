#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

namespace engine::network
{
#include OATPP_CODEGEN_BEGIN(ApiClient)

    class NetworkManagerApiClient final : public oatpp::web::client::ApiClient
    {
      public:
        API_CLIENT_INIT(NetworkManagerApiClient)

        API_CALL("GET", "/{path}", get, PATH(String, path))

        API_CALL("POST", "/{path}", post, PATH(String, path), BODY_STRING(String, body))

        API_CALL("PUT)", "/{path}", put, PATH(String, path), BODY_STRING(String, body))

        API_CALL("DELETE", "/{path}", del, PATH(String, path))
    };

#include OATPP_CODEGEN_END(ApiClient)

    class NetworkManager
    {
      public:
        explicit NetworkManager(const oatpp::String& base_url);
        ~NetworkManager() = default;

        oatpp::String get(const oatpp::String& path);
        oatpp::String post(const oatpp::String& path, const oatpp::String& body);
        oatpp::String put(const oatpp::String& path, const oatpp::String& body);
        oatpp::String del(const oatpp::String& path);

        bool is_connected();
        void set_base_url(const oatpp::String& base_url);

      private:
        std::shared_ptr<NetworkManagerApiClient> client_;
    };
} // namespace engine::network