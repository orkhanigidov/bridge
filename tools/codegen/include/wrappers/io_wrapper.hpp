/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

#pragma once

#include <string>
#include <ogdf/fileformats/GraphIO.h>

namespace codegen::wrappers
{
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
} // namespace codegen::wrappers
