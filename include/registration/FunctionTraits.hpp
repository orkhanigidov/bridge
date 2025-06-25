#pragma once

#include "pch.hpp"

namespace engine::reflection
{
    template <typename Func>
    struct FunctionTraits;

    template <typename Ret, typename... Args>
    struct FunctionTraits<Ret (*)(Args...)>
    {
        using return_type             = Ret;
        using args_tuple              = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);

        template <size_t N>
        using arg_type = typename std::tuple_element<N, args_tuple>::type;
    };

    template <typename Ret, typename T, typename... Args>
    struct FunctionTraits<Ret (T::*)(Args...)>
    {
        using return_type             = Ret;
        using class_type              = T;
        using args_tuple              = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);

        template <size_t N>
        using arg_type = typename std::tuple_element<N, args_tuple>::type;
    };

    template <typename Func>
    std::string extract_return_type()
    {
        using traits      = FunctionTraits<Func>;
        using return_type = typename traits::return_type;
        return typeid(return_type).name();
    }

    template <typename Func, size_t Index>
    void extract_param_types_impl(std::vector<std::string>& param_types);

    template <typename Func>
    std::vector<std::string> extract_param_types()
    {
        using traits = FunctionTraits<Func>;
        std::vector<std::string> param_types;

        if constexpr (traits::arity > 0)
        {
            extract_param_types_impl<Func, 0>(param_types);
        }

        return param_types;
    }

    template <typename Func, size_t Index>
    void extract_param_types_impl(std::vector<std::string>& param_types)
    {
        using traits = FunctionTraits<Func>;

        if constexpr (Index < traits::arity)
        {
            using param_type = typename traits::template arg_type<Index>;
            param_types.emplace_back(typeid(param_type).name());
            extract_param_types_impl<Func, Index + 1>(param_types);
        }
    }

    template <typename Func>
    std::vector<std::string> extract_param_names()
    {
        using traits = FunctionTraits<Func>;
        std::vector<std::string> param_names;

        for (size_t i = 0; i < traits::arity; ++i)
        {
            param_names.emplace_back(std::to_string(i));
        }

        return param_names;
    }
} // namespace engine::reflection