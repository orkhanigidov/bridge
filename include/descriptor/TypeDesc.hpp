#pragma once

#include "pch.hpp"

namespace engine::model
{
    enum class Type : std::uint8_t
    {
        Void,
        Bool,
        Int,
        Float,
        Double,
        String,
        Vector,
        Map,
        Object,
        Custom
    };

    inline std::string to_string(Type type)
    {
        switch (std::move(type))
        {
            case Type::Void:
                return "void";
            case Type::Bool:
                return "bool";
            case Type::Int:
                return "int";
            case Type::Float:
                return "float";
            case Type::Double:
                return "double";
            case Type::String:
                return "string";
            case Type::Vector:
                return "vector";
            case Type::Map:
                return "map";
            case Type::Object:
                return "object";
            case Type::Custom:
                return "custom";
            default:
                return "unknown";
        }
    }

    inline Type to_type(std::string_view type_str)
    {
        if (type_str == "void")
            return Type::Void;
        if (type_str == "bool")
            return Type::Bool;
        if (type_str == "int")
            return Type::Int;
        if (type_str == "float")
            return Type::Float;
        if (type_str == "double")
            return Type::Double;
        if (type_str == "string")
            return Type::String;
        if (type_str == "vector")
            return Type::Vector;
        if (type_str == "map")
            return Type::Map;
        if (type_str == "object")
            return Type::Object;
        return Type::Custom;
    }

    class TypeDesc
    {
      public:
        explicit TypeDesc(Type type) : type_(std::move(type)) {}

        explicit TypeDesc(std::string_view custom_type_name)
            : type_(Type::Custom), custom_type_name_(custom_type_name)
        {
        }

        std::string to_string() const
        {
            if (type_ == Type::Custom)
            {
                return custom_type_name_;
            }

            return model::to_string(type_);
        }

        Type type() const
        {
            return type_;
        }

      private:
        Type type_;
        std::string custom_type_name_;
    };
} // namespace engine::model