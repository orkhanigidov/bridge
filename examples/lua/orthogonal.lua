flags = GraphAttributes.nodeGraphics | GraphAttributes.nodeType |
        GraphAttributes.edgeGraphics | GraphAttributes.edgeType

G = Graph()
GA = GraphAttributes(G, flags)
read(GA, G, __input__)
scale_node_sizes(G, GA, _nodeSize)

pl = PlanarizationLayout()

crossMin = SubgraphPlanarizer()
ps = PlanarSubgraphFastInt()
ps:runs(_nRuns)
vei = VariableEmbeddingInserter()
vei:removeReinsert(_rrOption)

crossMin:setSubgraph(ps)
crossMin:setInserter(vei)
pl:setCrossMin(crossMin)

pl:setEmbedder(EmbedderMinDepthMaxFaceLayers())

ol = OrthoLayout()
ol:separation(_separation)
ol:cOverhang(_cOverhang)
pl:setPlanarLayouter(ol)

pl:call(GA)
write(GA, __output__ .. "/lua.graphml")
drawSVG(GA, __output__ .. "/lua.svg")