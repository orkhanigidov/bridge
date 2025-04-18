#ifndef CUSTOMTYPETRAITS_H
#define CUSTOMTYPETRAITS_H

#include <type_traits>

#include <ogdf/basic/Graph.h>

namespace traits {
    template<typename T>
    struct is_custom_type : std::false_type {
    };

    template<>
    struct is_custom_type<ogdf::node> : std::true_type {
    };

    template<>
    struct is_custom_type<ogdf::edge> : std::true_type {
    };

    template<typename T>
    inline constexpr bool is_custom_type_v = is_custom_type<T>::value;

    template<typename T>
    inline constexpr bool always_false_v = false;
}

#endif //CUSTOMTYPETRAITS_H
