#pragma once

#include "model/Method.hpp"
#include "pch.hpp"

namespace engine::serialization
{
    class RttrConverter final
    {
      public:
        RttrConverter() = delete;

        static std::optional<rttr::variant> fromJson(const nlohmann::json& json,
                                                     const rttr::type& type);
        static nlohmann::json toJson(const rttr::variant& variant);

        static std::vector<rttr::variant> prepareMethodArguments(const model::Method& method,
                                                                 const nlohmann::json& json);
    };
} // namespace engine::serialization