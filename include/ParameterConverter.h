#ifndef PARAMETERCONVERTER_H
#define PARAMETERCONVERTER_H

#include <string>

#include "TypeRegistry.h"
#include "CustomTypeTraits.h"

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
        } else if constexpr (traits::is_custom_type_v<T>) {
            return g_typeRegistry.getObject<T>(value);
        } else {
            static_assert(traits::always_false_v<T>, "Conversion not supported for this type");
        }
    }
};

#endif //PARAMETERCONVERTER_H
