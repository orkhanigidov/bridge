#include "engine/core/engine.hpp"

namespace engine::core
{
    void engine::initialize(const oatpp::String& host, v_uint16 port)
    {
        if (is_initialized_)
        {
            throw std::logic_error("Engine is already initialized");
        }

        if (host->empty())
        {
            throw std::invalid_argument("Host cannot be empty");
        }

        if (port < 1 || port > 65535)
        {
            throw std::out_of_range("Port must be between 1 and 65535");
        }

        try
        {
            network_manager_ = std::make_unique<network::network_manager>(host, port);
            is_initialized_  = true;

            std::cout << "Engine initialized successfully on " << host->c_str() << ":" << port
                      << std::endl;
        }
        catch (const std::exception& e)
        {
            network_manager_.reset();
            throw std::runtime_error("Failed to initialize engine: " + std::string(e.what()));
        }
    }

    void engine::start()
    {
        if (!is_initialized_)
        {
            throw std::logic_error("Engine must be initialized before starting");
        }

        if (is_running_)
        {
            throw std::logic_error("Engine is already running");
        }

        try
        {
            // network_manager_->start();
            is_running_ = true;

            std::cout << "Engine started successfully" << std::endl;
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to start engine: " + std::string(e.what()));
        }
    }

    void engine::shutdown()
    {
        if (!is_initialized_)
        {
            return;
        }

        try
        {
            if (is_running_)
            {
                is_running_ = false;
                if (network_manager_)
                {
                    // network_manager_->shutdown();
                }
                std::cout << "Engine shutdown successfully" << std::endl;
            }

            if (network_manager_)
            {
                // network_manager_.->shutdown();
                network_manager_.reset();
            }

            is_initialized_ = false;
            std::cout << "Engine shutdown successfully" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during engine shutdown: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown error during engine shutdown" << std::endl;
        }
    }


} // namespace engine::core