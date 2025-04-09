#include "../include/GeneratedRegistry.h"

#include <sstream>
#include <stdexcept>

#include <ogdf/fileformats/GraphIO.h>

Registry::Registry() {
    initializeObjects();
    initializeMethods();
}

void Registry::initializeObjects() {
    generated::registerObjects(this);
}

void Registry::initializeMethods() {
    generated::registerAllMethods(this);

    memberMethods_["graph"]["writeGML"] = [](const std::vector<std::string> &params) {
        if (params.empty()) {
            throw std::invalid_argument("Missing filename parameters");
        }
        const bool success = ogdf::GraphIO::write(g_graph, params[0], ogdf::GraphIO::writeGML);
        return success ? "Graph written to " + params[0] : "Failed to write graph to " + params[0];
    };

    memberMethods_["graph"]["readGML"] = [](const std::vector<std::string> &params) {
        if (params.empty()) {
            throw std::invalid_argument("Missing filename parameters");
        }
        const bool success = ogdf::GraphIO::read(g_graph, params[0], ogdf::GraphIO::readGML);
        return success ? "Graph read from " + params[0] : "Failed to read graph from " + params[0];
    };
}

std::string Registry::invokeMethod(const std::string &objectId, const std::string &methodId,
                                   const std::vector<std::string> &args) {
    if (objectId.empty()) {
        if (const auto it = globalMethods_.find(methodId); it != globalMethods_.end()) {
            try {
                return it->second(args);
            } catch (std::exception &e) {
                return "Error: " + std::string(e.what());
            }
        }
        return "Error: Unknown method '" + methodId + "'";
    }

    const auto objectIt = memberMethods_.find(objectId);
    if (objectIt == memberMethods_.end()) {
        return "Error: Unknown instance '" + objectId + "'";
    }

    const auto methodIt = objectIt->second.find(methodId);
    if (methodIt == objectIt->second.end()) {
        return "Error: Unknown method '" + methodId + "' for object '" + objectId + "'";
    }

    try {
        return methodIt->second(args);
    } catch (std::exception &e) {
        return "Error: " + std::string(e.what());
    }
}

std::string Registry::listAvailableMethods() const {
    std::stringstream ss;
    ss << "Available methods:\n";

    ss << "Global methods:\n";
    for (const auto &[methodId, _]: globalMethods_) {
        ss << "- " << methodId << "\n";
    }

    for (const auto &[objectId, methods]: memberMethods_) {
        ss << "Member '" << objectId << "' methods:\n";
        for (const auto &[methodId, _]: methods) {
            ss << "- " << methodId << "\n";
        }
    }
    return ss.str();
}
