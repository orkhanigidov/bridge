#include <iostream>

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

int main() {
    return 0;
}
