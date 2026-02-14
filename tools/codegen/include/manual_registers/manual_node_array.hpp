/**
 * Project: Engine
 * Tool: Codegen
 * File: manual_node_array.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "bindings/lua/member_registrar.hpp"
#include "bindings/lua/utils/sol_trait_macros.hpp"

#include <ogdf/basic/NodeArray.h>

#include <sol/sol.hpp>

namespace sol
{
    DISABLE_SOL_AUTOMAGICAL(ogdf::NodeArray<int>);
}

using namespace engine::bindings::lua;
using namespace ogdf;

inline void register_manual_node_array(sol::state& lua)
{
    MemberRegistrar<NodeArray<int>, MemoryOwnership::Lua> node_array(lua, "NodeArrayInt");

    node_array.add_shared_constructors<sol::types<>, sol::types<const Graph&>, sol::types<const Graph&, const int&>, sol::types<const NodeArray<int>&>>();
}
