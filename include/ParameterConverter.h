#ifndef PARAMETERCONVERTER_H
#define PARAMETERCONVERTER_H

#include <string>
#include <stdexcept>
#include <type_traits>

#include <ogdf/basic/Graph.h>

extern ogdf::Graph g_graph;

class ParameterConverter {
public:
    template<typename T>
    static T convert(const std::string &value) {
        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<T, int>) {
            return std::stoi(value);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(value);
        } else if constexpr (std::is_same_v<T, bool>) {
            return value == "true";
        } else if constexpr (std::is_same_v<T, ogdf::node>) {
            const int id = std::stoi(value);
            for (ogdf::node n: g_graph.nodes) {
                if (n->index() == id) {
                    return n;
                }
            }
            throw std::runtime_error("Node with index " + std::to_string(id) + " not found");
        } else if constexpr (std::is_same_v<T, ogdf::edge>) {
            const int id = std::stoi(value);
            for (ogdf::edge e: g_graph.edges) {
                if (e->index() == id) {
                    return e;
                }
            }
            throw std::runtime_error("Edge with index " + std::to_string(id) + " not found");
        } else {
            static_assert(always_false_v<T>, "Conversion not supported for this type");
        }
    }

private:
    template<typename T>
    static constexpr bool always_false_v = false;
};

#endif //PARAMETERCONVERTER_H
