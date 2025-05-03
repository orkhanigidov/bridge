#pragma once

#include "Method.h"
#include <mutex>
#include <rttr/registration>

namespace engine {

    class Engine {
    public:
        static Engine& instance();
        ~Engine() = default;

        Engine(const Engine&)            = delete;
        Engine& operator=(const Engine&) = delete;

        void registerMethods();
        std::vector<Method> getAvailableMethods() const;

        nlohmann::json executeMethod(const std::string& methodName, const nlohmann::json& params);

    private:
        Engine() = default;

        static std::mutex instanceMutex;
        static std::unique_ptr<Engine> instancePtr;

        std::unordered_map<std::string, rttr::method> methodRegistry;
        mutable std::mutex methodMutex;
    };

} // namespace engine
