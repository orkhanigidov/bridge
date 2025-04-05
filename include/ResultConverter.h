#ifndef RESULTCONVERTER_H
#define RESULTCONVERTER_H

#include <string>
#include <sstream>
#include <type_traits>

#include <ogdf/basic/Graph.h>

class ResultConverter {
public:
    template<typename T>
    static std::string convert(const T &result) {
        if constexpr (std::is_same_v<T, std::string>) {
            return result;
        } else if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(result);
        } else if constexpr (std::is_same_v<T, ogdf::node>) {
            return std::to_string(result->index());
        } else if constexpr (std::is_same_v<T, ogdf::edge>) {
            std::stringstream ss;
            ss << "edge(" << result->source()->index() << "->" << result->target()->index() << ")";
            return ss.str();
        } else {
            static_assert(always_false_v<T>, "Conversion not supported for this type");
        }
    }

private:
    template<typename T>
    static constexpr bool always_false_v = false;
};

#endif //RESULTCONVERTER_H
