#include "core/Engine.hpp"
#include "network/NetworkManager.hpp"
#include "pch.hpp"
#include "reflection/MethodRegistrar.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"

nlohmann::json processMessage(const std::string& message)
{
    try
    {
        nlohmann::json request = nlohmann::json::parse(message);

        if (!request.contains("method") || !request["method"].is_string())
        {
            throw std::invalid_argument("Missing or invalid 'method' field");
        }

        std::string methodName = request["method"];
        if (methodName == "getRegisteredMethods")
        {
            return engine::core::Engine::getInstance().getRegisteredMethods();
        }

        nlohmann::json params = request.value("params", nlohmann::json::object());

        return engine::core::Engine::getInstance().executeMethod(methodName, params);
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid JSON format: " + std::string(e.what()));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Error processing message: " + std::string(e.what()));
    }
}

std::atomic_bool running{true};
void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

int main(int argc, char* argv[])
{
    try
    {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        std::string endpoint = "tcp://*:5555";
        if (argc > 1)
        {
            endpoint = argv[1];
        }

        std::cout << "Starting engine service..." << std::endl;

        engine::reflection::ReflectionRegistry::instance().register_all_from_rttr();

        engine::network::NetworkManager networkManager(processMessage);
        networkManager.initialize(endpoint);
        networkManager.startMessageLoop();

        std::cout << "Engine service running on " << endpoint << std::endl;
        std::cout << "Press Ctrl+C to exit" << std::endl;

        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Shutting down engine service..." << std::endl;
        networkManager.shutdown();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}