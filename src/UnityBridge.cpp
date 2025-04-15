#include "../include/UnityBridge.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "../include/Registry.h"

extern Registry g_registry;

std::string handleRequestImpl(const std::string &path, const std::string &args) {
    std::string objectId;
    std::string methodId = path;

    const size_t firstSlash = path.find_first_of('/');
    const size_t lastSlash = path.find_last_of('/');

    if (firstSlash != std::string::npos) {
        if (firstSlash != lastSlash) {
            objectId = path.substr(firstSlash + 1, lastSlash - firstSlash - 1);
            methodId = path.substr(lastSlash + 1);
        } else {
            methodId = path.substr(lastSlash + 1);
            objectId = "";
        }
    }

    std::vector<std::string> methodArgs;
    std::istringstream iss(args);
    std::string arg;
    while (std::getline(iss, arg, ',')) {
        methodArgs.push_back(arg);
    }

    if (methodId == "methods") {
        return g_registry.listAvailableMethods();
    }

    return g_registry.invokeMethod(objectId, methodId, methodArgs);
}

static char *g_LastResult = nullptr;

void FreeLastResult() {
    if (g_LastResult) {
        delete[] g_LastResult;
        g_LastResult = nullptr;
    }
}

const char *HandleRequest(const char *path, const char *args) {
    FreeLastResult();

    try {
        const std::string result = handleRequestImpl(path ? path : "", args ? args : "");

        g_LastResult = new char[result.length() + 1];
        strcpy_s(g_LastResult, result.length() + 1, result.c_str());

        return g_LastResult;
    } catch (const std::exception &e) {
        const std::string errorMsg = "Error: " + std::string(e.what());
        g_LastResult = new char[errorMsg.length() + 1];
        strcpy_s(g_LastResult, errorMsg.length() + 1, errorMsg.c_str());
        return g_LastResult;
    }
}

std::string handleRequest(const std::string &path, const std::string &args) {
    return handleRequestImpl(path, args);
}
