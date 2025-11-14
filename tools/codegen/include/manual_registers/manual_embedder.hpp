#pragma once

#include "bindings/lua/member_registrar.hpp"
#include "bindings/lua/utils/sol_trait_macros.hpp"

#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>

#include <sol/sol.hpp>

using namespace engine::bindings::lua;
using namespace ogdf;

namespace sol
{
    DISABLE_SOL_AUTOMAGICAL(EmbedderMinDepthMaxFaceLayers);
}

inline void register_manual_embedder(sol::state& lua)
{
    MemberRegistrar<EmbedderMinDepthMaxFaceLayers, MemoryOwnership::Lua> embedder(lua, "EmbedderMinDepthMaxFaceLayers");

    embedder.add_shared_constructors<sol::types<>>();
    embedder.add_bases<EmbedderModule>();
}
