#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/Parameter.h"
#include "../../include/pch.h"

namespace engine::operation
{
    Parameter parameterFromJson(const nlohmann::json& parameterValue)
    {
        if (parameterValue.is_object() && parameterValue.contains("ref"))
            return Parameter::createReference(parameterValue["ref"]);

        if (parameterValue.is_number_integer()) return Parameter(parameterValue.get<int>());
        if (parameterValue.is_number_float()) return Parameter(parameterValue.get<double>());
        if (parameterValue.is_boolean()) return Parameter(parameterValue.get<bool>());
        if (parameterValue.is_string()) return Parameter(parameterValue.get<std::string>());

        throw ExecutionError(ErrorType::InvalidConversion, "Unsupported parameter type: " + parameterValue.dump());
    }
} // namespace engine::operation
