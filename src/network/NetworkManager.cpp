#include "../../include/network/NetworkManager.h"

#include <iostream>

namespace engine {

NetworkManager::NetworkManager(MessageHandler handler)
    : messageHandler(std::move(handler)) {}

NetworkManager::~NetworkManager() { shutdown(); }

void NetworkManager::initialize(const std::string &endpoint) {
  try {
    zmqContext = std::make_unique<zmq::context_t>(1);
    zmqSocket =
        std::make_unique<zmq::socket_t>(*zmqContext, zmq::socket_type::rep);
    zmqSocket->bind(endpoint);
    std::cout << "NetworkManager initialized on endpoint: " << endpoint
              << std::endl;
  } catch (const zmq::error_t &e) {
    throw std::runtime_error("Failed to initialize ZeroMQ: " +
                             std::string(e.what()));
  }
}

void NetworkManager::shutdown() {
  stopMessageLoop();

  if (zmqSocket) {
    try {
      zmqSocket->close();
      zmqSocket.reset();
    } catch (const zmq::error_t &e) {
      std::cerr << "Error closing ZeroMQ socket: " << e.what() << std::endl;
    }
  }

  if (zmqContext) {
    try {
      zmqContext->close();
      zmqContext.reset();
    } catch (const zmq::error_t &e) {
      std::cerr << "Error closing ZeroMQ context: " << e.what() << std::endl;
    }
  }
}

void NetworkManager::startMessageLoop() {
  if (running) {
    std::cout << "Message loop is already running" << std::endl;
    return;
  }

  if (!zmqSocket || !zmqContext) {
    throw std::runtime_error(
        "Cannot start message loop: NetworkManager not initialized");
  }

  running = true;
  messageThread =
      std::make_unique<std::thread>(&NetworkManager::messageLoop, this);
  std::cout << "Message loop started" << std::endl;
}

void NetworkManager::stopMessageLoop() {
  if (!running) {
    return;
  }

  running = false;

  if (messageThread && messageThread->joinable()) {
    messageThread->join();
    messageThread.reset();
    std::cout << "Message loop stopped" << std::endl;
  }
}

bool NetworkManager::isRunning() const { return running; }

void NetworkManager::messageLoop() {
  const auto timeout = std::chrono::milliseconds(100);

  while (running) {
    try {
      zmq::message_t request;
      if (zmqSocket->recv(request, zmq::recv_flags::dontwait)) {
        std::cout << "Received message of size: " << request.size()
                  << std::endl;
        std::string messageStr(static_cast<char *>(request.data()),
                               request.size());
        handleIncomingMessage(messageStr);
      } else {
        std::this_thread::sleep_for(timeout);
      }
    } catch (const zmq::error_t &e) {
      if (e.num() == EAGAIN || e.num() == EINTR) {
        std::this_thread::sleep_for(timeout);
      } else if (running) {
        std::cerr << "ZeroMQ error in message loop: " << e.what() << std::endl;
        std::this_thread::sleep_for(timeout);
      }
    } catch (const std::exception &e) {
      if (running) {
        std::cerr << "Unexpected error in message loop: " << e.what()
                  << std::endl;
        std::this_thread::sleep_for(timeout);
      }
    }
  }
}

void NetworkManager::handleIncomingMessage(const std::string &message) {
  try {
    const nlohmann::json response = messageHandler(message);

    const std::string responseStr = response.dump();
    zmq::message_t zmqResponse(responseStr.size());
    memcpy(zmqResponse.data(), responseStr.data(), responseStr.size());

    zmqSocket->send(zmqResponse, zmq::send_flags::none);
  } catch (const std::exception &e) {
    const nlohmann::json errorResponse = {
        {"error", true},
        {"message", std::string("Error processing request: ") + e.what()}};

    const std::string errorStr = errorResponse.dump();
    zmq::message_t zmqError(errorStr.size());
    memcpy(zmqError.data(), errorStr.data(), errorStr.size());

    try {
      zmqSocket->send(zmqError, zmq::send_flags::none);
    } catch (const zmq::error_t &e) {
      std::cerr << "Failed to send error response: " << e.what() << std::endl;
    }
  }
}

} // namespace engine