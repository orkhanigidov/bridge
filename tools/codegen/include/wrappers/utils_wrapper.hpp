/**
 * Project: Engine
 * Tool: Codegen
 * File: utils_wrapper.hpp
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

#include <algorithm>
#include <iostream>
#include <limits>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph_d.h>

using namespace ogdf;

inline void normalize(const Graph& G, GraphAttributes& GA, double target_size)
{
    if (G.numberOfNodes() == 0)
    {
        return;
    }

    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double min_z = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double max_y = std::numeric_limits<double>::lowest();
    double max_z = std::numeric_limits<double>::lowest();

    const bool has_node_graphics = GA.has(GraphAttributes::nodeGraphics);
    const bool has_3d = GA.has(GraphAttributes::threeD);

    if (!has_node_graphics)
    {
        std::cerr << "Warning: GraphAttributes::nodeGraphics is not enabled. Cannot normalize coordinates." << std::endl;
        return;
    }

    for (const node& v : G.nodes)
    {
        double x = GA.x(v);
        double y = GA.y(v);

        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);

        if (has_3d)
        {
            double z = GA.z(v);
            min_z = std::min(min_z, z);
            max_z = std::max(max_z, z);
        }
    }

    if (!has_3d)
    {
        min_z = 0.0;
        max_z = 0.0;
    }

    double current_width = max_x - min_x;
    double current_height = max_y - min_y;
    double current_depth = max_z - min_z;

    const double center_x = min_x + current_width / 2.0;
    const double center_y = min_y + current_height / 2.0;
    const double center_z = min_z + current_depth / 2.0;

    if (current_width == 0.0)
    {
        current_width = 1.0;
    }

    if (current_height == 0.0)
    {
        current_height = 1.0;
    }

    if (current_depth == 0.0)
    {
        current_depth = 1.0;
    }

    double max_dimension = std::max(current_width, current_height);
    if (has_3d)
    {
        max_dimension = std::max(max_dimension, current_depth);
    }

    for (const node& v : G.nodes)
    {
        const double x = GA.x(v);
        const double y = GA.y(v);

        const double norm_x = (x - center_x) / max_dimension;
        const double norm_y = (y - center_y) / max_dimension;

        const double new_x = norm_x * target_size;
        const double new_y = norm_y * target_size;

        GA.x(v) = new_x;
        GA.y(v) = new_y;

        if (has_3d)
        {
            const double z = GA.z(v);
            const double norm_z = (z - center_z) / max_dimension;
            const double new_z = norm_z * target_size;
            GA.z(v) = new_z;
        }
    }
}

inline void scale_node_sizes(const Graph& G, GraphAttributes& GA, const double size)
{
    for (const node v : G.nodes)
    {
        GA.width(v) /= size;
        GA.height(v) /= size;
    }
}

inline void set_node_sizes(const Graph& G, GraphAttributes& GA, const double size)
{
    for (const node v : G.nodes)
    {
        GA.width(v) = size;
        GA.height(v) = size;
    }
}

inline void set_node_ranks(const Graph& G, NodeArray<int>& rank, const sol::table& lua_table)
{
    for (const node v : G.nodes)
    {
        int node_id = v->index() + 1;
        rank[v] = lua_table.get<int>(node_id);
    }
}