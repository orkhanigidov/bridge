#ifndef RESULTCONVERTER_H
#define RESULTCONVERTER_H

#include <string>

#include "TypeRegistry.h"
#include "CustomTypeTraits.h"

class ResultConverter {
public:
    template<typename T>
    static std::string convert(const T &result) {
        if constexpr (std::is_same_v<T, std::string>) {
            return result;
        } else if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(result);
        } else if constexpr (traits::is_custom_type_v<T>) {
            return g_typeRegistry.registerObject(result);
        } else {
            static_assert(traits::always_false_v<T>, "Conversion not supported for this type");
        }
    }
};

#endif //RESULTCONVERTER_H
