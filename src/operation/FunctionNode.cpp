#include "../../include/operation/FunctionNode.h"

#include "../../include/operation/Result.h"
#include "../../include/pch.h"
#include "../../include/reflection/ReflectionRegistry.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::operation
{
    FunctionNode::FunctionNode(std::string name) : InvokableNode(std::move(name), NodeType::Function)
    {
        const model::Method* method = reflection::ReflectionRegistry::getInstance().getMethod(name);

        m_method = method->getMethod();
    }

    Result FunctionNode::invoke()
    {
        if (!isValid())
            throw std::runtime_error("Function not found: " + getName());

        const std::vector<rttr::variant> parameters = serialization::RttrConverter::prepareMethodArguments(m_method);
        const std::vector<rttr::argument> args(parameters.begin(), parameters.end());
        const rttr::variant result = m_method.invoke_variadic({}, args);

        // if (result.is_type<void>())
        // return Result(serialization::RttrConverter::toJson(result));

        return Result(result);
    }
} // namespace engine::operation