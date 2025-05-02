#include "../include/core/Engine.h"
#include "../include/core/Registration.h"
#include "../include/network/NetworkManager.h"

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

std::unique_ptr<engine::NetworkManager> g_networkManager;

void signalHandler(const int signal) {
  std::cout << "\nReceived signal " << signal << std::endl;

  if (g_networkManager && g_networkManager->isRunning()) {
    std::cout << "Shutting down network engine..." << std::endl;
    g_networkManager->shutdown();
  }

  exit(signal);
}

void setupSignalHandlers() {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
}

void printUsage(const char *programName) {
  std::cout << "Usage: " << programName << " [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << " --endpoint <url>  Network endpoint (default: tcp://*:5555)"
            << std::endl;
  std::cout << " --help, -h        Show this help message" << std::endl;
}

int main(const int argc, char *argv[]) {
  try {
    std::cout << "Starting Engine application..." << std::endl;

    std::string endpoint = "tcp://*:5555";
    for (int i = 1; i < argc; i++) {
      if (std::string arg = argv[i]; arg == "--endpoint" && i + 1 < argc) {
        endpoint = argv[i + 1];
        i++;
      } else if (arg == "--help" || arg == "-h") {
        printUsage(argv[0]);
        return 0;
      }
    }

    setupSignalHandlers();

    engine::Engine &engineInstance = engine::Engine::instance();

    engine::registerTypes();
    engineInstance.registerMethods();

    g_networkManager = std::make_unique<engine::NetworkManager>(
        [&engineInstance](const std::string &message) -> nlohmann::json {
          try {
            nlohmann::json request = nlohmann::json::parse(message);

            if (!request.contains("method")) {
              return {{"error", true},
                      {"message", "Missing 'method' field in request"}};
            }

            const std::string methodName = request["method"];
            const nlohmann::json params =
                request.value("params", nlohmann::json::object());

            return engineInstance.executeMethod(methodName, params);
          } catch (const std::exception &e) {
            return {{"error", true},
                    {"message", std::string("Request error: ") + e.what()}};
          }
        });

    std::cout << "Using endpoint: " << endpoint << std::endl;
    g_networkManager->initialize(endpoint);
    g_networkManager->startMessageLoop();

    std::cout << "Engine running. Press Ctrl+C to exit." << std::endl;

    while (g_networkManager->isRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Engine stopped." << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
}