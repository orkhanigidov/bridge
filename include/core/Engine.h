#pragma once

#include "../pch.h"

namespace engine::core
{

class Engine
{
  public:
    ~Engine() = default;

    static Engine &getInstance();

    nlohmann::json executeMethod(const std::string &methodName, const nlohmann::json &params);

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

  private:
    Engine() = default;

    static std::mutex s_instanceMutex;
    static std::unique_ptr<Engine> s_instancePtr;
};

} // namespace engine::core