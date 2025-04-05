#include <iostream>
#include <sstream>
#include <vector>

#include "../include/Registry.h"

extern Registry g_registry;

std::string handleRequest(const std::string &path, const std::string &params) {
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
        return g_registry.listAllMethods();
    }

    return g_registry.execute(instanceName, methodName, paramList);
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
