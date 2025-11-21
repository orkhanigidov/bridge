flags = GraphAttributes.nodeGraphics | GraphAttributes.edgeGraphics |
        GraphAttributes.nodeLabel | GraphAttributes.edgeStyle |
        GraphAttributes.nodeStyle | GraphAttributes.nodeTemplate

G = Graph()
GA = GraphAttributes(G, flags)
read(GA, G, __input__)

sl = SugiyamaLayout()
sl:setRanking(OptimalRanking())
sl:setCrossMin(MedianHeuristic())

ohl = OptimalHierarchyLayout()
ohl:layerDistance(30.0)
ohl:nodeDistance(25.0)
ohl:weightBalancing(0.8)
sl:setLayout(ohl)

sl:call(GA)
write(GA, __output__)
write(GA, "tests/temp/hierarchical.svg")