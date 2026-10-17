#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphList.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/fmmm/FMMMOptions.h>
#include <ogdf/fileformats/GraphIO.h>
#include <iostream>
#include <string>

using namespace ogdf;

int main()
{
	Graph G;
	GraphAttributes GA(G);
	if (!GraphIO::read(G, __input__, GraphIO::readGraphML))
	{
		std::cerr << "Could not load " << __input__ << std::endl;
		return 1;
	}

	for (node v : G.nodes)
		GA.width(v) = GA.height(v) = 5.0;

	FMMMLayout fmmm;

	fmmm.useHighLevelOptions(true);
	fmmm.unitEdgeLength(15.0);
	fmmm.newInitialPlacement(true);
	fmmm.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);

	fmmm.call(GA);
	GraphIO::write(GA, std::string(__output__) + "/cpp.graphml", GraphIO::writeGraphML);
	GraphIO::write(GA, std::string(__output__) + "/cpp.svg", GraphIO::drawSVG);

	return 0;
}