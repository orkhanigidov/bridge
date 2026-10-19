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
ohl:layerDistance(_layerDistance)
ohl:nodeDistance(_nodeDistance)
ohl:weightBalancing(_weightBalancing)
sl:setLayout(ohl)

sl:call(GA)
write(GA, __output__ .. "/lua.graphml")
drawSVG(GA, __output__ .. "/lua.svg")