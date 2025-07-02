#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/client/ApiClient.hpp>

namespace engine::network
{
#include OATPP_CODEGEN_BEGIN(ApiClient)

    class api_client final : public oatpp::web::client::ApiClient
    {
      public:
        API_CLIENT_INIT(api_client)

        API_CALL("GET", "/{path}", get, PATH(String, path))
        API_CALL("POST", "/{path}", post, PATH(String, path), BODY_STRING(String, body))
        API_CALL("PUT", "/{path}", put, PATH(String, path), BODY_STRING(String, body))
        API_CALL("DELETE", "/{path}", del, PATH(String, path))
    };

#include OATPP_CODEGEN_END(ApiClient)
} // namespace engine::network