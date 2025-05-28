#include "../../include/pch.h"

#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/operation/ObjectPool.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::operation
{
    MethodNode::MethodNode(std::string_view name) : InvokableNode(name, NodeType::Method) {}

    void MethodNode::resolve()
    {
        const auto name = method().get_declaring_type().get_name().to_string();

        if (!ObjectPool::instance().has_object(name))
            throw std::runtime_error("Object not found in pool: " + name);

        instance_ = ObjectPool::instance().get_object(name);
    }

    Result MethodNode::invoke()
    {
        if (!is_valid())
            throw std::runtime_error("Method not found: " + name_);

        const auto prepared_args = serialization::RttrConverter::prepare_arguments(method_, parameters_);
        const std::vector<rttr::argument> args{prepared_args.begin(), prepared_args.end()};

        auto result = rttr::variant();

        if (method().is_static())
            result = method().invoke_variadic({}, args);
        else
            result = method().invoke_variadic(instance_, args);

        if (!result.is_valid())
            throw std::runtime_error("Invalid invocation result for function: " + name_);

        if (result.is_type<void>())
            return Result(rttr::variant());

        return Result(serialization::RttrConverter::to_json(result));
    }
} // namespace engine::operation