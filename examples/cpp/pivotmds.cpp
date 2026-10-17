#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/energybased/PivotMDS.h>
#include <iostream>
#include <string>

using namespace ogdf;

int main()
{
        Graph G;
        GraphAttributes GA(G, GraphAttributes::nodeGraphics |
                                  GraphAttributes::edgeGraphics |
                                  GraphAttributes::nodeLabel |
                                  GraphAttributes::edgeStyle |
                                  GraphAttributes::nodeStyle |
                                  GraphAttributes::nodeTemplate |
                                  GraphAttributes::threeD);
        if (!GraphIO::read(GA, G, __input__, GraphIO::readGraphML))
        {
                std::cerr << "Could not load " << __input__ << std::endl;
                return 1;
        }

        PivotMDS mds;
        mds.call(GA);

        GraphIO::write(GA, std::string(__output__) + "/cpp.graphml", GraphIO::writeGraphML);
        GraphIO::write(GA, std::string(__output__) + "/cpp.svg", GraphIO::drawSVG);

        return 0;
}