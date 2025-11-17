/**
 * Project: Engine
 * Tool: Codegen
 * File: manual_embedder.hpp
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
