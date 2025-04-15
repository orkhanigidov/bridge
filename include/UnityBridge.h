#ifndef UNITYBRIDGE_H
#define UNITYBRIDGE_H

#include <string>

// C++ interface
std::string handleRequest(const std::string &path, const std::string &args);

// C-style exported interface for Unity
extern "C" {
__declspec(dllexport) const char *HandleRequest(const char *path, const char *args);

__declspec(dllexport) void FreeLastResult();
}

#endif //UNITYBRIDGE_H
