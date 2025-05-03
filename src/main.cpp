#include "../include/core/Engine.h"
#include "../include/network/NetworkManager.h"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

std::unique_ptr<engine::NetworkManager> networkManager;

void signalHandler(const int signal) {
    std::cout << "\nReceived signal " << signal << std::endl;

    if (networkManager) {
        std::cout << "Shutting down network engine..." << std::endl;
        networkManager->shutdown();
    }
}

nlohmann::json processMessage(const std::string& message, engine::Engine& engine) {
    try {
        auto request = nlohmann::json::parse(message);

        if (!request.contains("method")) {
            return {{"error", true}, {"message", "Missing 'method' field in request"}};
        }

        const auto& methodName = request["method"].get<std::string>();
        const auto& params     = request.value("params", nlohmann::json::object());

        return engine.executeMethod(methodName, params);
    } catch (const std::exception& e) {
        return {{"error", true}, {"message", "Request error: " + std::string(e.what())}};
    }
}

struct CommandLineArgs {
    std::string endpoint = "tcp://*:5555";
    bool showHelp        = false;
};

CommandLineArgs parseArgs(int argc, char* argv[]) {
    CommandLineArgs args;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "--endpoint" || arg == "-e") && i + 1 < argc) {
            args.endpoint = argv[++i];
            i++;
        } else if (arg == "--help" || arg == "-h") {
            args.showHelp = true;
        }
    }
    return args;
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n"
              << "Options:\n"
              << " --endpoint, -e <url>  Network endpoint (default: "
                 "tcp://*:5555)\n"
              << " --help, -h            Show this help message" << std::endl;
}

int main(const int argc, char* argv[]) {
    try {
        std::cout << "Starting Engine application..." << std::endl;

        auto args = parseArgs(argc, argv);

        if (args.showHelp) {
            printUsage(argv[0]);
            return 0;
        }

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        auto& engineInstance = engine::Engine::instance();
        engineInstance.registerMethods();

        networkManager = std::make_unique<engine::NetworkManager>(
            [&engineInstance](const std::string& message) { return processMessage(message, engineInstance); });

        std::cout << "Using endpoint: " << args.endpoint << std::endl;
        networkManager->initialize(args.endpoint);
        networkManager->startMessageLoop();

        std::cout << "Engine running. Press Ctrl+C to exit." << std::endl;

        while (networkManager && networkManager->isRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Engine stopped." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
