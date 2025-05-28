#include "../../include/pch.h"

#include "../../include/operation/FunctionNode.h"
#include "../../include/operation/Result.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::operation
{
    FunctionNode::FunctionNode(std::string_view name) : InvokableNode(name, NodeType::Function) {}

    Result FunctionNode::invoke()
    {
        if (!is_valid())
            throw std::runtime_error("Function not found: " + name_);

        const auto prepared_args = serialization::RttrConverter::prepare_arguments(method_, parameters_);
        const std::vector<rttr::argument> args{prepared_args.begin(), prepared_args.end()};

        const auto result = method_.invoke_variadic({}, args);

        if (!result.is_valid())
            throw std::runtime_error("Invalid invocation result for function: " + name_);

        if (result.is_type<void>())
            return Result(rttr::variant());

        return Result(serialization::RttrConverter::to_json(result));
    }
} // namespace engine::operation