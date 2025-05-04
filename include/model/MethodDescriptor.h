#pragma once

#include "../pch.h"

namespace engine::model
{

struct MethodParameterDescriptor final
{
    std::string name;
    std::string type;
    std::string defaultValue;
};

struct MethodDescriptor final
{
    std::string name;
    std::string returnType;
    std::vector<MethodParameterDescriptor> parameters;
    std::string category;
    std::string description;
};

} // namespace engine::model