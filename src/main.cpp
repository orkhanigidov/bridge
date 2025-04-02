#include <functional>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>

ogdf::Graph g_graph;

class ParameterConverter {
public:
    template<typename T>
    static T convert(const std::string &param) {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(param);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(param);
        } else if constexpr (std::is_same_v<T, bool>) {
            return param == "true";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return param;
        } else {
            throw std::invalid_argument("Unsupported parameter type");
        }
    }
};

class ResultConverter {
public:
    template<typename T>
    static std::string convert(const T &result) {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, bool>) {
            return std::to_string(result);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return result;
        } else {
            throw std::invalid_argument("Unsupported return type");
        }
    }
};

class Registry {
    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)> > methods;

public:
    Registry() {
        registerMethods();
    }

    void registerMethods() {
        methods["writeGML"] = [](const std::vector<std::string> &params) {
            if (params.empty()) {
                throw std::invalid_argument("Missing required parameters");
            }
            const bool success = ogdf::GraphIO::write(g_graph, params[0], ogdf::GraphIO::writeGML);
            return success ? "Graph written to " + params[0] : "Failed to write graph to " + params[0];
        };

        methods["readGML"] = [](const std::vector<std::string> &params) {
            if (params.empty()) {
                throw std::invalid_argument("Missing required parameters");
            }
            const bool success = ogdf::GraphIO::read(g_graph, params[0], ogdf::GraphIO::readGML);
            return success ? "Graph read from " + params[0] : "Failed to read graph from " + params[0];
        };
    }

    std::string execute(const std::string &methodName, const std::vector<std::string> &params) {
        if (const auto it = methods.find(methodName); it != methods.end()) {
            try {
                return it->second(params);
            } catch (std::exception &e) {
                return "Error: " + std::string(e.what());
            }
        }
        return "Error: Unknown method '" + methodName + "'";
    }

    std::string listMethods() const {
        std::stringstream ss;
        ss << "List of available methods:\n";
        for (const auto &method: methods) {
            ss << "- " << method.first << "\n";
        }
        return ss.str();
    }
};

Registry g_registry;

std::string handleRequest(const std::string &path, const std::string &params) {
    std::string methodName = path;
    if (const size_t lastSlash = path.find_last_of('/'); lastSlash != std::string::npos) {
        methodName = methodName.substr(lastSlash + 1);
    }

    std::vector<std::string> paramList;
    std::istringstream iss(params);
    std::string param;
    while (std::getline(iss, param, ',')) {
        paramList.push_back(param);
    }

    if (methodName == "methods") {
        return g_registry.listMethods();
    }

    return g_registry.execute(methodName, paramList);
}

int main() {
    std::cout << handleRequest("/graph/methods", "") << std::endl;
    std::cout << handleRequest("/graph/writeGML", "output.gml") << std::endl;
    std::cout << handleRequest("/graph/readGML", "output.gml") << std::endl;

    return 0;
}
