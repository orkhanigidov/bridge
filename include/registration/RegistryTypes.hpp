#pragma once

#include "model/ClassDesc.hpp"
#include "model/FuncDesc.hpp"
#include "pch.hpp"

namespace engine::registration
{
    using ClassMap = std::unordered_map<std::string, std::unique_ptr<model::ClassDesc>>;
    using FuncMap  = std::unordered_map<std::string, std::unique_ptr<model::FuncDesc>>;
} // namespace engine::registration