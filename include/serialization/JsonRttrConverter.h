#pragma once

#include <nlohmann/json.hpp>
#include <rttr/variant.h>

namespace engine::serialization
{

class JsonRttrConverter
{
  public:
    static std::optional<rttr::argument> jsonToArgument(const nlohmann::json &jsonValue, const rttr::type &paramType);
    static nlohmann::json variantToJson(const rttr::variant &result);

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