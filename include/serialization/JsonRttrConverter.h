#pragma once

#include <rttr/variant.h>

#include <nlohmann/json.hpp>

namespace engine::serialization
{

class JsonRttrConverter
{
  public:
    static std::optional<rttr::argument> jsonToArgument(const nlohmann::json &jsonValue, const rttr::type &paramType);
    static nlohmann::json variantToJson(const rttr::variant &result);

    static nlohmann::json errorToJson(const std::string &message, int code = -1);

  private:
    JsonRttrConverter() = default;
    ~JsonRttrConverter() = default;

    JsonRttrConverter(const JsonRttrConverter &) = delete;
    JsonRttrConverter &operator=(const JsonRttrConverter &) = delete;
};

} // namespace engine::serialization