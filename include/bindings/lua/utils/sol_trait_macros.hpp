/**
 * Project: Engine
 * File: sol_trait_macros.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

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
