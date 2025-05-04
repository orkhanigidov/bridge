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

  private:
    MethodRegistry() = default;
    ~MethodRegistry() = default;

    MethodRegistry(const MethodRegistry &) = delete;
    MethodRegistry &operator=(const MethodRegistry &) = delete;

    void registerMethod(const rttr::method &method, const std::string &methodName);

    static std::mutex instanceMutex;
    static std::unique_ptr<MethodRegistry> instancePtr;

    mutable std::mutex methodMutex;
    std::unordered_map<std::string, model::MethodDescriptor> methodMap;
};

} // namespace engine::core::reflection

#define REGISTER_METHOD(Class, Method, Parameters, Category, Description)                                              \
    rttr::registration::class_<Class>(#Class).method(#Method, &Class::Method)(                                         \
        rttr::parameter_names Parameters, rttr::metadata("category", Category),                                        \
        rttr::metadata("description", Description))

#define REGISTER_FUNCTION(Function, Parameters, Category, Description)                                                 \
    rttr::registration::method(#Function, &Function)(rttr::parameter_names Parameters,                                 \
                                                     rttr::metadata("category", Category),                             \
                                                     rttr::metadata("description", Description))