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
