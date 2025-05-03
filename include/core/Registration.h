#pragma once

#include "Method.h"
#include <rttr/registration>

#define REGISTER_METHOD(Class, Method, Parameters, Category, Description)                                        \
    rttr::registration::class_<Class>(#Class).method(#Method, &Class::Method)(rttr::parameter_names(Parameters), \
        rttr::metadata("category", Category), rttr::metadata("description", Description));                       \
    rttr::registration::method(#Method "_info", []() -> engine::Method {                                         \
        engine::Method method;                                                                                   \
        method.name        = #Method;                                                                            \
        method.parameters  = #Parameters;                                                                        \
        method.category    = Category;                                                                           \
        method.description = Description;                                                                        \
        return method;                                                                                           \
    })

#define REGISTER_FUNCTION(Function, Parameters, Category, Description)                     \
    rttr::registration::method(#Function, &Function)(rttr::parameter_names(Parameters),    \
        rttr::metadata("category", Category), rttr::metadata("description", Description)); \
    rttr::registration::method(#Function "_info", []() -> engine::Method {                 \
        engine::Method method;                                                             \
        method.name        = #Function;                                                    \
        method.parameters  = #Parameters;                                                  \
        method.category    = Category;                                                     \
        method.description = Description;                                                  \
        return method;                                                                     \
    })
