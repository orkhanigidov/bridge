#pragma once

#include "api_client.hpp"

namespace engine::network
{
    class network_manager final
    {
      public:
        explicit network_manager(const oatpp::String& host, v_uint16 port);

        oatpp::String get(const oatpp::String& path) const;
        oatpp::String post(const oatpp::String& path, const oatpp::String& body) const;
        oatpp::String put(const oatpp::String& path, const oatpp::String& body) const;
        oatpp::String del(const oatpp::String& path) const;

        bool is_connected() const;

      private:
        std::shared_ptr<api_client> api_client_;
    };
} // namespace engine::network