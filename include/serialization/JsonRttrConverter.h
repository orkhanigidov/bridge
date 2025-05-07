#pragma once

#include "../pch.h"

namespace engine::serialization
{

class JsonRttrConverter
{
  public:
    static std::vector<rttr::variant> convertMethodParams(const rttr::method &method, const nlohmann::json &params);
    static std::optional<rttr::variant> jsonToVariant(const nlohmann::json &jsonValue, const rttr::type &type);
    static nlohmann::json variantToJson(const rttr::variant &returnValue);

    static nlohmann::json errorToJson(const std::string &message, int code = -1);

    JsonRttrConverter(const JsonRttrConverter &) = delete;
    JsonRttrConverter &operator=(const JsonRttrConverter &) = delete;
    JsonRttrConverter(JsonRttrConverter &&) = delete;
    JsonRttrConverter &operator=(JsonRttrConverter &&) = delete;

  private:
    JsonRttrConverter() = default;
    ~JsonRttrConverter() = default;
};

} // namespace engine::serialization