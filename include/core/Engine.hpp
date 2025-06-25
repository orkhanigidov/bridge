#pragma once

#include "network/NetworkManager.hpp"

namespace engine::core
{
    class Engine
    {
      public:
        ~Engine() = default;

        static Engine& instance()
        {
            static Engine instance;
            return instance;
        }

        void initialize(const std::string& host = "localhost", int port = 8080);
        void start();
        void shutdown();

      private:
        std::unique_ptr<network::NetworkManager> network_manager_;
        std::string host_;
        int port_;
        bool initialized_{false};
        bool running_{false};

        Engine() = default;
    };
} // namespace engine::core