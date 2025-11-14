#pragma once

#include <type_traits>

namespace engine::bindings::lua::utils
{
#define DISABLE_SOL_CONTAINER(Type) \
    template<> \
    struct is_container<Type> : std::false_type {}

#define DISABLE_SOL_AUTOMAGICAL(Type) \
    template<> \
    struct is_automagical<Type> : std::false_type {}
} // namespace engine::bindings::lua::utils
