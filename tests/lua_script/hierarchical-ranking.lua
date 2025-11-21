flags = GraphAttributes.nodeGraphics | GraphAttributes.edgeGraphics |
        GraphAttributes.nodeLabel | GraphAttributes.edgeStyle |
        GraphAttributes.nodeStyle | GraphAttributes.nodeTemplate

r = {
    0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 9, 10, 10, 11, 12, 12,
    13, 14, 14, 15, 16, 17, 18, 18, 19, 19, 20, 21, 22, 22,
    22, 23, 23, 23, 23, 24, 25, 26, 27, 27, 27, 28, 29, 29,
    29, 30, 30, 31, 31, 31, 32, 33, 33, 34, 34, 35, 35, 35,
    35, 0, 1, 2, 3, 5, 6, 7, 8, 10, 11, 12, 14, 15, 16, 18,
    19, 20, 21, 22, 23, 25, 27, 29, 30, 31, 32, 33, 34, 35, -1
}

G = Graph()
GA = GraphAttributes(G, flags)
read(GA, G, __input__)

rank = NodeArrayInt(G)
set_node_ranks(G, rank, r)

sl = SugiyamaLayout()
sl:setCrossMin(MedianHeuristic())
sl:arrangeCCs(false)

ohl = OptimalHierarchyLayout()
ohl:layerDistance(30.0)
ohl:nodeDistance(25.0)
ohl:weightBalancing(0.7)
sl:setLayout(ohl)

sl:call(GA, rank)
write(GA, __output__)
write(GA, "tests/temp/hierarchical-ranking.svg")