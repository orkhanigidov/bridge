#pragma once

#define REGISTER_METHOD(Class, Method, Category, Description)                  \
  rttr::registration::class_<Class>(#Class).method(#Method, &Class::Method)(   \
      rttr::metadata("category", Category),                                    \
      rttr::metadata("description", Description));                             \
  rttr::registration::method(                                                  \
      #Method "_info", []() -> engine::MethodDescriptor {                      \
        engine::MethodDescriptor desc;                                         \
        desc.name = #Method;                                                   \
        desc.category = Category;                                              \
        desc.description = Description;                                        \
        return desc;                                                           \
      })(rttr::detail::metadata("category", "internal"))

namespace engine {

void registerTypes();

} // namespace engine