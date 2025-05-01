#pragma once

#include <atomic>
#include <nlohmann/json.hpp>
#include <thread>
#include <zmq.hpp>

namespace engine {

class NetworkManager {
public:
  using MessageHandler = std::function<nlohmann::json(const std::string &)>;

  explicit NetworkManager(MessageHandler handler);
  ~NetworkManager();

  void initialize(const std::string &endpoint = "tcp://*:5555");
  void shutdown();

  void startMessageLoop();
  void stopMessageLoop();

  bool isRunning() const;

private:
  std::unique_ptr<zmq::context_t> zmqContext;
  std::unique_ptr<zmq::socket_t> zmqSocket;
  MessageHandler messageHandler;
  std::atomic_bool running{false};
  std::unique_ptr<std::thread> messageThread;

  void messageLoop();

  void handleIncomingMessage(const std::string &message);
};

} // namespace engine