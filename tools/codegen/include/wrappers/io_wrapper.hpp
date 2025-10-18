#pragma once

#include <ogdf/fileformats/GraphIO.h>

using namespace ogdf;

namespace codegen::wrappers
{
    inline bool read(Graph& G, const string& filename)
    {
        return GraphIO::read(G, filename);
    }

    inline bool read(GraphAttributes& GA, Graph& G, const string& filename)
    {
        return GraphIO::read(GA, G, filename);
    }

    inline bool write(const Graph& G, const string& filename)
    {
        return GraphIO::write(G, filename);
    }

    inline bool write(const GraphAttributes& GA, const string& filename)
    {
        return GraphIO::write(GA, filename);
    }
} // namespace codegen::wrappers
