flags = GraphAttributes.nodeGraphics | GraphAttributes.edgeGraphics

G = Graph()
GA = GraphAttributes(G, flags)
read(G, __input__)
set_node_sizes(G, GA, _nodeSize)

fmmm = FMMMLayout()

fmmm:useHighLevelOptions(_useHighLevelOptions)
fmmm:unitEdgeLength(_unitEdgeLength)
fmmm:newInitialPlacement(_newInitialPlacement)
fmmm:qualityVersusSpeed(_qualityVersusSpeed)

fmmm:call(GA)
write(GA, __output__ .. "/lua.graphml")
drawSVG(GA, __output__ .. "/lua.svg")