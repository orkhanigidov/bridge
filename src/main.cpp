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
        } else if constexpr (std::is_same_v<T, ogdf::node> || std::is_same_v<T, ogdf::edge>) {
            return std::to_string(result->index());
        } else {
            throw std::invalid_argument("Unsupported return type");
        }
    }
};

class Registry {
    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)> > methods;

public:
    Registry() {
        registerFunctionsAndMethods();
    }

    template<typename Ret, typename Class, typename... Args>
    void registerMethod(const std::string &methodName, Ret (Class::*function)(Args...), Class &instance) {
        methods[methodName] = [&instance, function, methodName](const std::vector<std::string> &params) {
            if (params.size() < sizeof...(Args)) {
                throw std::invalid_argument(methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
            }
            std::index_sequence_for<Args...> indices{};
            return callMethod<Ret, Class, Args...>(instance, function, params, indices);
        };
    }

    template<typename Ret, typename Class>
    void registerMethod(const std::string &methodName, Ret (Class::*function)(), Class &instance) {
        methods[methodName] = [&instance, function](const std::vector<std::string> &params) {
            Ret result = (instance.*function)();
            return ResultConverter::convert(result);
        };
    }

    template<typename Ret, typename Class>
    void registerMethod(const std::string &methodName, Ret (Class::*function)() const, const Class &instance) {
        methods[methodName] = [&instance, function](const std::vector<std::string> &params) {
            Ret result = (instance.*function)();
            return ResultConverter::convert(result);
        };
    }

    template<typename Ret, typename Class, typename... Args, std::size_t... I>
    static std::string callMethod(Class &instance, Ret (Class::*function)(Args...),
                                  const std::vector<std::string> &params, std::index_sequence<I...>) {
        if constexpr (std::is_same_v<Ret, void>) {
            (instance.*function)(ParameterConverter::convert<Args>(params[I])...);
            return "Operation completed successfully";
        } else {
            Ret result = (instance.*function)(ParameterConverter::convert<Args>(params[I])...);
            return ResultConverter::convert(result);
        }
    }

    template<typename Ret, typename... Args>
    void registerFunction(const std::string &methodName, Ret (*function)(Args...)) {
        methods[methodName] = [function, methodName](const std::vector<std::string> &params) {
            if (params.size() < sizeof...(Args)) {
                throw std::invalid_argument(methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
            }
            std::index_sequence_for<Args...> indices{};
            return callFunction<Ret, Args...>(function, params, indices);
        };
    }

    template<typename Ret>
    void registerFunction(const std::string &methodName, Ret (*function)()) {
        methods[methodName] = [function](const std::vector<std::string> &params) {
            Ret result = (*function)();
            return ResultConverter::convert(result);
        };
    }

    template<typename Ret, typename... Args, std::size_t... I>
    static std::string callFunction(Ret (*function)(Args...), const std::vector<std::string> &params,
                                    std::index_sequence<I...>) {
        if constexpr (std::is_same_v<Ret, void>) {
            (*function)(ParameterConverter::convert<Args>(params[I])...);
            return "Operation completed successfully";
        } else {
            Ret result = (*function)(ParameterConverter::convert<Args>(params[I])...);
            return ResultConverter::convert(result);
        }
    }

    void registerFunctionsAndMethods() {
        registerFunction("setSeed", &ogdf::setSeed);
        registerFunction("randomNumber", &ogdf::randomNumber);
        registerMethod("newNode", &ogdf::Graph::newNode, g_graph);
        registerMethod(
            "newEdge", static_cast<ogdf::edge (ogdf::Graph::*)(ogdf::node, ogdf::node, int)>(&ogdf::Graph::newEdge),
            g_graph);
        registerMethod("numberOfNodes", &ogdf::Graph::numberOfNodes, g_graph);
        registerMethod("numberOfEdges", &ogdf::Graph::numberOfEdges, g_graph);

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
    std::cout << handleRequest("/setSeed", "1") << std::endl;
    std::cout << handleRequest("/randomNumber", "1, 10") << std::endl;
    std::cout << handleRequest("/graph/newNode", "1") << std::endl;
    std::cout << handleRequest("/graph/newNode", "2") << std::endl;
    std::cout << handleRequest("/graph/newEdge", "1, 2, 1") << std::endl;
    std::cout << handleRequest("/graph/numberOfNodes", "") << std::endl;
    std::cout << handleRequest("/graph/numberOfEdges", "") << std::endl;
    std::cout << handleRequest("/graph/writeGML", "output.gml") << std::endl;
    std::cout << handleRequest("/graph/readGML", "output.gml") << std::endl;

    return 0;
}
