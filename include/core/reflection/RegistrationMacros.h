#pragma once

#include "../../pch.h"

// Macro to begin registration block
#define BEGIN_RTTR_REGISTRATION                                                                                        \
    RTTR_REGISTRATION                                                                                                  \
    {                                                                                                                  \
        using namespace rttr;

// Macro to end registration block
#define END_RTTR_REGISTRATION }

// Macro for registering a function with parameters and metadata
#define REGISTER_FUNCTION(func, returnType, ...)                                                                       \
    registration::method(#func, select_overload<returnType(__VA_ARGS__)>(&func))

// Macro to add default arguments to a function or method registration
#define WITH_DEFAULT_ARGS(...) (default_arguments(__VA_ARGS__))

// Macro to add parameter names to a function or method registration
#define WITH_PARAMETER_NAMES(...) (parameter_names(__VA_ARGS__))

// Macro to add category metadata
#define WITH_CATEGORY(category) (metadata("category", category))

// Macro to add description metadata
#define WITH_DESCRIPTION(description) (metadata("description", description))

// Macro for registering a class
#define REGISTER_CLASS(TypeName) registration::class_<TypeName>(#TypeName)

// Macro for adding a constructor to a class registration
#define WITH_CONSTRUCTOR() .constructor<>()

// Macro for registering a class method with parameters and metadata
#define REGISTER_METHOD(TypeName, method, returnType, ...)                                                             \
    .method(#method, select_overload<returnType(__VA_ARGS__)>(&TypeName::method))