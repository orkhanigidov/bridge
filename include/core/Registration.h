#pragma once

#include "MethodDescriptor.h"

#include <rttr/registration>

#define REGISTER_METHOD(Class, Method, Category, Description)                  \
  rttr::registration::class_<Class>(#Class).method(#Method, &Class::Method)(   \
      rttr::metadata("category", Category),                                    \
      rttr::metadata("description", Description));                             \
  rttr::registration::method(#Method "_info",                                  \
                             []() -> engine::MethodDescriptor {                \
                               engine::MethodDescriptor desc;                  \
                               desc.name = #Method;                            \
                               desc.category = Category;                       \
                               desc.description = Description;                 \
                               return desc;                                    \
                             })

#define REGISTER_FUNCTION(Function, Category, Description)                     \
  rttr::registration::method(#Function, &Function)(                            \
      rttr::metadata("category", Category),                                    \
      rttr::metadata("description", Description));                             \
  rttr::registration::method(#Function "_info",                                \
                             []() -> engine::MethodDescriptor {                \
                               engine::MethodDescriptor desc;                  \
                               desc.name = #Function;                          \
                               desc.category = Category;                       \
                               desc.description = Description;                 \
                               return desc;                                    \
                             })