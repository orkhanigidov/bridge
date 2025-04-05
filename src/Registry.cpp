#include "../include/Registry.h"

#include <sstream>
#include <stdexcept>

#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>

Registry::Registry() {
    registerInstances();
    registerAllMethods();
}

void Registry::registerInstances() {
    registerInstance("graph", &g_graph);
}

void Registry::registerAllMethods() {
    registerGlobalMethod("setSeed", &ogdf::setSeed);
    registerGlobalMethod("randomNumber", &ogdf::randomNumber);

    registerInstanceMethod<>("graph", "newNode", &ogdf::Graph::newNode);
    registerInstanceMethod<>("graph", "newEdge",
                             static_cast<ogdf::edge (ogdf::Graph::*)(ogdf::node, ogdf::node, int)>(&
                                 ogdf::Graph::newEdge));
    registerInstanceMethod<>("graph", "numberOfNodes", &ogdf::Graph::numberOfNodes);
    registerInstanceMethod<>("graph", "numberOfEdges", &ogdf::Graph::numberOfEdges);

    instanceMethods_["graph"]["writeGML"] = [](const std::vector<std::string> &params) {
        if (params.empty()) {
            throw std::invalid_argument("Missing filename parameters");
        }
        const bool success = ogdf::GraphIO::write(g_graph, params[0], ogdf::GraphIO::writeGML);
        return success ? "Graph written to " + params[0] : "Failed to write graph to " + params[0];
    };

    instanceMethods_["graph"]["readGML"] = [](const std::vector<std::string> &params) {
        if (params.empty()) {
            throw std::invalid_argument("Missing filename parameters");
        }
        const bool success = ogdf::GraphIO::read(g_graph, params[0], ogdf::GraphIO::readGML);
        return success ? "Graph read from " + params[0] : "Failed to read graph from " + params[0];
    };
}

std::string Registry::execute(const std::string &instanceName, const std::string &methodName,
                              const std::vector<std::string> &params) {
    if (instanceName.empty()) {
        if (const auto it = globalMethods_.find(methodName); it != globalMethods_.end()) {
            try {
                return it->second(params);
            } catch (std::exception &e) {
                return "Error: " + std::string(e.what());
            }
        }
        return "Error: Unknown method '" + methodName + "'";
    }

    const auto instanceIt = instanceMethods_.find(instanceName);
    if (instanceIt == instanceMethods_.end()) {
        return "Error: Unknown instance '" + instanceName + "'";
    }

    const auto methodIt = instanceIt->second.find(methodName);
    if (methodIt == instanceIt->second.end()) {
        return "Error: Unknown method '" + methodName + "' for instance '" + instanceName + "'";
    }

    try {
        return methodIt->second(params);
    } catch (std::exception &e) {
        return "Error: " + std::string(e.what());
    }
}


std::string Registry::listAllMethods() const {
    std::stringstream ss;
    ss << "Available methods:\n";

    ss << "Global methods:\n";
    for (const auto &[name, _]: globalMethods_) {
        ss << "- " << name << "\n";
    }

    for (const auto &[instanceName, methods]: instanceMethods_) {
        ss << "Instance '" << instanceName << "' methods:\n";
        for (const auto &[name, _]: methods) {
            ss << "- " << name << "\n";
        }
    }
    return ss.str();
}
