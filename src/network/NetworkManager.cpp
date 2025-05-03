#include "../../include/network/NetworkManager.h"

#include <iostream>

namespace engine {

    NetworkManager::NetworkManager(MessageHandler handler) : messageHandler(std::move(handler)) {
        if (!messageHandler) {
            throw std::invalid_argument("Message handler cannot be null");
        }
    }

    NetworkManager::~NetworkManager() {
        shutdown();
    }

    void NetworkManager::initialize(const std::string& endpoint) {
        try {
            if (zmqSocket || zmqContext) {
                shutdown();
            }

            zmqContext = std::make_unique<zmq::context_t>(1);
            zmqSocket  = std::make_unique<zmq::socket_t>(*zmqContext, zmq::socket_type::rep);
            zmqSocket->bind(endpoint);
            std::cout << "NetworkManager initialized on endpoint: " << endpoint << std::endl;
        } catch (const zmq::error_t& e) {
            throw std::runtime_error("Failed to initialize ZeroMQ: " + std::string(e.what()));
        }
    }

    void NetworkManager::shutdown() {
        stopMessageLoop();

        if (zmqSocket) {
            try {
                zmqSocket->close();
            } catch (const zmq::error_t& e) {
                std::cerr << "Error closing ZeroMQ socket: " << e.what() << std::endl;
            }
            zmqSocket.reset();
        }

        if (zmqContext) {
            try {
                zmqContext->close();
            } catch (const zmq::error_t& e) {
                std::cerr << "Error closing ZeroMQ context: " << e.what() << std::endl;
            }
            zmqContext.reset();
        }
    }

    void NetworkManager::startMessageLoop() {
        if (running) {
            return;
        }

        if (!zmqSocket || !zmqContext) {
            throw std::runtime_error("Cannot start message loop: NetworkManager not initialized");
        }

        running       = true;
        messageThread = std::make_unique<std::thread>(&NetworkManager::messageLoop, this);
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

    bool NetworkManager::isRunning() const {
        return running;
    }

    void NetworkManager::messageLoop() const {
        const auto timeout = std::chrono::milliseconds(100);

        while (running) {
            try {
                zmq::message_t request;
                if (zmqSocket->recv(request, zmq::recv_flags::dontwait)) {
                    std::string messageStr(static_cast<char*>(request.data()), request.size());
                    handleIncomingMessage(messageStr);
                } else {
                    std::this_thread::sleep_for(timeout);
                }
            } catch (const zmq::error_t& e) {
                if (e.num() == EAGAIN || e.num() == EINTR) {
                    std::this_thread::sleep_for(timeout);
                } else if (running) {
                    std::cerr << "ZeroMQ error: " << e.what() << std::endl;
                    std::this_thread::sleep_for(timeout);
                }
            } catch (const std::exception& e) {
                if (running) {
                    std::cerr << "Unexpected error: " << e.what() << std::endl;
                    std::this_thread::sleep_for(timeout);
                }
            }
        }
    }

    void NetworkManager::handleIncomingMessage(const std::string& message) const {
        nlohmann::json response;

        try {
            response = messageHandler(message);
        } catch (const std::exception& e) {
            response = {{"error", true}, {"message", std::string("Error processing request: ") + e.what()}};
        }

        try {
            const std::string responseStr = response.dump();
            zmqSocket->send(zmq::buffer(responseStr), zmq::send_flags::none);
        } catch (const zmq::error_t& e) {
            std::cerr << "Failed to send response: " << e.what() << std::endl;
        }
    }

} // namespace engine
