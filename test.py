import json

import zmq

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

# request = {
#     "pipeline": "graph_pipeline",
#     "steps": [
#         {"new": "Graph", "as": "G"},
#         {"call": "randomSimpleGraph", "with": {"G": "$G", "n": 10, "m": 20}},
#         {"new": "DfsAcyclicSubgraph", "as": "DAS"},
#         {"call": "$DAS.callAndReverse_2", "with": {"G": "$G"}},
#         {"call": "writeGraphML_1", "with": {"G": "$G", "os": "output-acyclic-graph.graphml"}}
#     ]
# }

request = {
    "pipelines": [
        {
            "pipeline": "graph_pipeline",
            "steps": [
                {"new": "Graph", "as": "G"},
                {"new": "GraphAttributes", "as": "GA", "with": {"G": "$G"}},
                {"call": "randomSimpleGraph", "with": {"G": "$G", "n": 10, "m": 20}},
                # {"call": "write_GraphML", "with": {"G": "$G", "filename": "output-unix-history-hierarchical.gml"}},
                {"new": "SugiyamaLayout", "as": "SL"},
                {"new": "OptimalHierarchyLayout", "as": "ohl"},
                {"call": "$ohl.layerDistance", "with": {"x": 30.0}},
                {"call": "$ohl.nodeDistance", "with": {"x": 25.0}},
                {"call": "$ohl.weightBalancing", "with": {"w": 0.8}},
                {"call": "setLayout", "with": {"sl": "$SL", "ohl": "$ohl"}},
                {"call": "call", "with": {"sl": "$SL", "GA": "$GA"}},
                {"call": "write", "with": {"GA": "$GA", "filename": "output-unix-history-hierarchical.gml"}},
            ]
        }
    ]
}

socket.send_string(json.dumps(request))

response = socket.recv_string()
print(json.loads(response))

socket.close()
context.term()
