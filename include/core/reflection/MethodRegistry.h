#pragma once

#include "../../model/MethodDescriptor.h"

#include <mutex>
#include <rttr/registration>

namespace engine::core::reflection
{

class MethodRegistry
{
  public:
    static MethodRegistry &getInstance();

    void registerAll();
    std::vector<model::MethodDescriptor> getRegisteredMethods() const;

    bool hasMethod(const std::string &methodName) const;
    model::MethodDescriptor findMethod(const std::string &methodName) const;

    MethodRegistry(const MethodRegistry &) = delete;
    MethodRegistry &operator=(const MethodRegistry &) = delete;
    MethodRegistry(MethodRegistry &&) = delete;
    MethodRegistry &operator=(MethodRegistry &&) = delete;

  private:
    MethodRegistry() = default;
    ~MethodRegistry() = default;

    void registerMethod(const rttr::method &method, const std::string &methodName);

    static std::mutex instanceMutex;
    static std::unique_ptr<MethodRegistry> instancePtr;

    mutable std::mutex methodMutex;
    std::unordered_map<std::string, model::MethodDescriptor> methodMap;
};

} // namespace engine::core::reflection