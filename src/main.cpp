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
        } else if constexpr (std::is_same_v<T, ogdf::node>) {
            const int id = std::stoi(param);
            for (ogdf::node n: g_graph.nodes) {
                if (n->index() == id) {
                    return n;
                }
            }
            throw std::runtime_error("Node with index " + param + " not found");
        } else if constexpr (std::is_same_v<T, ogdf::edge>) {
            const int id = std::stoi(param);
            for (ogdf::edge e: g_graph.edges) {
                if (e->index() == id) {
                    return e;
                }
            }
            throw std::runtime_error("Edge with index " + param + " not found");
        } else {
            throw std::invalid_argument("Unsupported parameter type");
        }
    }
};

class ResultConverter {
public:
    template<typename T>
    static std::string convert(const T &result) {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
            return std::to_string(result);
        } else if constexpr (std::is_same_v<T, bool>) {
            return result ? "true" : "false";
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
    std::unordered_map<std::string, void *> instances_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::function<std::string(
        const std::vector<std::string> &)> > > instanceMethods_;
    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)> > globalMethods_;

public:
    Registry() {
        registerInstances();
        registerFunctionsAndMethods();
    }

    void registerInstance(const std::string &name, void *instance) {
        if (instance == nullptr) {
            throw std::invalid_argument("Cannot register null instance for '" + name + "'");
        }
        instances_[name] = instance;
    }

    void *getInstance(const std::string &name) const {
        const auto it = instances_.find(name);
        if (it == instances_.end()) {
            return nullptr;
        }
        return it->second;
    }

    template<typename Ret, typename Class, typename... Args>
    void registerMethod(const std::string &instanceName, const std::string &methodName,
                        Ret (Class::*function)(Args...)) {
        void *instancePtr = getInstance(instanceName);
        if (!instancePtr) {
            throw std::runtime_error("Instance '" + instanceName + "' not found");
        }
        auto *instance = static_cast<Class *>(instancePtr);

        instanceMethods_[instanceName][methodName] = [instance, function, methodName
                ](const std::vector<std::string> &params) {
                    if (params.size() < sizeof...(Args)) {
                        throw std::invalid_argument(
                            methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
                    }
                    return callMethod<Ret, Class, Args...>(*instance, function, params,
                                                           std::index_sequence_for<Args...>{});
                };
    }

    template<typename Ret, typename Class>
    void registerMethod(const std::string &instanceName, const std::string &methodName, Ret (Class::*function)()) {
        void *instancePtr = getInstance(instanceName);
        if (!instancePtr) {
            throw std::runtime_error("Instance '" + instanceName + "' not found");
        }
        auto *instance = static_cast<Class *>(instancePtr);

        instanceMethods_[instanceName][methodName] = [instance, function](const std::vector<std::string> &) {
            Ret result = (instance->*function)();
            return ResultConverter::convert(result);
        };
    }

    template<typename Ret, typename Class>
    void registerMethod(const std::string &instanceName, const std::string &methodName,
                        Ret (Class::*function)() const) {
        void *instancePtr = getInstance(instanceName);
        if (!instancePtr) {
            throw std::runtime_error("Instance '" + instanceName + "' not found");
        }
        auto *instance = static_cast<Class *>(instancePtr);

        instanceMethods_[instanceName][methodName] = [instance, function](const std::vector<std::string> &) {
            Ret result = (instance->*function)();
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
        globalMethods_[methodName] = [function, methodName](const std::vector<std::string> &params) {
            if (params.size() < sizeof...(Args)) {
                throw std::invalid_argument(methodName + " requires " + std::to_string(sizeof...(Args)) + " arguments");
            }
            return callFunction<Ret, Args...>(function, params, std::index_sequence_for<Args...>{});
        };
    }

    template<typename Ret>
    void registerFunction(const std::string &methodName, Ret (*function)()) {
        globalMethods_[methodName] = [function](const std::vector<std::string> &) {
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

    void registerInstances() {
        registerInstance("graph", &g_graph);
    }

    void registerFunctionsAndMethods() {
        registerFunction("setSeed", &ogdf::setSeed);
        registerFunction("randomNumber", &ogdf::randomNumber);
        registerMethod("graph", "newNode", &ogdf::Graph::newNode);
        registerMethod("graph",
                       "newEdge",
                       static_cast<ogdf::edge (ogdf::Graph::*)(ogdf::node, ogdf::node, int)>(&ogdf::Graph::newEdge));
        registerMethod("graph", "numberOfNodes", &ogdf::Graph::numberOfNodes);
        registerMethod("graph", "numberOfEdges", &ogdf::Graph::numberOfEdges);

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

    std::string execute(const std::string &instanceName, const std::string &methodName,
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

    std::string listMethods() const {
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
};

Registry g_registry;

std::string handleRequest(const std::string &path, const std::string &params) {
    static Registry registry;
    std::string instanceName;
    std::string methodName = path;

    const size_t firstSlash = path.find_first_of('/');
    if (const size_t lastSlash = path.find_last_of('/');
        firstSlash != std::string::npos && lastSlash != std::string::npos && firstSlash != lastSlash) {
        instanceName = path.substr(firstSlash + 1, lastSlash - firstSlash - 1);
        methodName = path.substr(lastSlash + 1);
    } else if (lastSlash != std::string::npos) {
        methodName = path.substr(lastSlash + 1);
        instanceName = "";
    }

    std::vector<std::string> paramList;
    std::istringstream iss(params);
    std::string param;
    while (std::getline(iss, param, ',')) {
        paramList.push_back(param);
    }

    if (methodName == "methods") {
        return registry.listMethods();
    }

    return registry.execute(instanceName, methodName, paramList);
}

int main() {
    std::cout << handleRequest("/methods", "") << std::endl;
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
