flags = GraphAttributes.nodeGraphics | GraphAttributes.nodeType |
        GraphAttributes.edgeGraphics | GraphAttributes.edgeType

G = Graph()
GA = GraphAttributes(G, flags)
read(GA, G, __input__)
scale_node_sizes(G, GA, 2)

pl = PlanarizationLayout()

crossMin = SubgraphPlanarizer()
ps = PlanarSubgraphFastInt()
ps:runs(100)
vei = VariableEmbeddingInserter()
vei:removeReinsert(All)

crossMin:setSubgraph(ps)
crossMin:setInserter(vei)
pl:setCrossMin(crossMin)

pl:setEmbedder(EmbedderMinDepthMaxFaceLayers())

ol = OrthoLayout()
ol:separation(20.0)
ol:cOverhang(0.4)
pl:setPlanarLayouter(ol)

pl:call(GA)
write(GA, __output__)
drawSVG(GA, "tests/temp/orthogonal.svg")