#pragma once

#include "engine/network/network_manager.hpp"
#include "pch.hpp"

namespace engine::core
{
    class engine final
    {
      public:
        static engine& instance()
        {
            static engine instance;
            return instance;
        }

        void initialize(const oatpp::String& host, v_uint16 port);
        void start();
        void shutdown();

        bool is_initialized() const noexcept
        {
            return is_initialized_;
        }

        bool is_running() const noexcept
        {
            return is_running_;
        }

      private:
        std::unique_ptr<network::network_manager> network_manager_;
        bool is_initialized_{false};
        bool is_running_{false};
    };
} // namespace engine::core