#pragma once

#include "../../pch.h"

#include "../../model/MethodDescriptor.h"

namespace engine::core::reflection
{

class MethodRegistry
{
  public:
    ~MethodRegistry() = default;

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

    void registerMethod(const rttr::method &method);

    static std::mutex s_instanceMutex;
    static std::unique_ptr<MethodRegistry> s_instancePtr;

    mutable std::mutex m_methodMutex;
    std::unordered_map<std::string, model::MethodDescriptor> m_methodMap;
};

} // namespace engine::core::reflection