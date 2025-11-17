/**
* Project: Engine
 * Tool: Codegen
 * File: io_wrapper.hpp
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

#include <string>
#include <ogdf/fileformats/GraphIO.h>

inline bool read(ogdf::Graph& G, const std::string& filename)
{
    return ogdf::GraphIO::read(G, filename);
}

inline bool read(ogdf::GraphAttributes& GA, ogdf::Graph& G, const std::string& filename)
{
    return ogdf::GraphIO::read(GA, G, filename);
}

inline bool write(const ogdf::Graph& G, const std::string& filename)
{
    return ogdf::GraphIO::write(G, filename);
}

inline bool write(const ogdf::GraphAttributes& GA, const std::string& filename)
{
    return ogdf::GraphIO::write(GA, filename);
}
