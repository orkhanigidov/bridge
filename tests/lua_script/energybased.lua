flags = GraphAttributes.nodeGraphics | GraphAttributes.edgeGraphics

G = Graph()
GA = GraphAttributes(G, flags)
read(G, __input__)
set_node_sizes(G, GA, 5.0)

fmmm = FMMMLayout()

fmmm:useHighLevelOptions(true)
fmmm:unitEdgeLength(15.0)
fmmm:newInitialPlacement(true)
fmmm:qualityVersusSpeed(GorgeousAndEfficient)

fmmm:call(GA)
write(GA, __output__)
write(GA, "tests/temp/energybased.svg")