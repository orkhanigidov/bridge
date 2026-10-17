flags = GraphAttributes.nodeGraphics | GraphAttributes.edgeGraphics |
        GraphAttributes.nodeLabel | GraphAttributes.edgeStyle |
        GraphAttributes.nodeStyle | GraphAttributes.nodeTemplate | GraphAttributes.threeD

G = Graph()
GA = GraphAttributes(G, flags)
read(GA, G, __input__)

pmds = PivotMDS()

pmds:call(GA)
write(GA, __output__ .. "/lua.graphml")
drawSVG(GA, __output__ .. "/lua.svg")