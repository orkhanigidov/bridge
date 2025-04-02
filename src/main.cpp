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

class ResultConverter {
public:
    template<typename T>
    static std::string convert(const T &result) {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, bool>) {
            return std::to_string(result);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return result;
        } else {
            throw std::invalid_argument("Unsupported return type");
        }
    }
};

int main() {
    return 0;
}
