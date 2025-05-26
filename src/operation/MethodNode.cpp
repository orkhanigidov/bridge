#include "../../include/operation/MethodNode.h"

#include "../../include/operation/ObjectNode.h"
#include "../../include/pch.h"
#include "../../include/reflection/ReflectionRegistry.h"
#include "../../include/serialization/RttrConverter.h"

namespace engine::operation
{
    MethodNode::MethodNode(std::string name) : InvokableNode(std::move(name), NodeType::Method)
    {
        const model::Method* method = reflection::ReflectionRegistry::getInstance().getMethod(name);

        m_method = method->getMethod();
    }

    Result MethodNode::invoke()
    {
        if (!isValid())
            throw std::runtime_error("Method not found: " + getName());

        const std::vector<rttr::variant> parameters = serialization::RttrConverter::prepareMethodArguments(m_method);
        const std::vector<rttr::argument> args(parameters.begin(), parameters.end());

        rttr::variant result;
        if (m_method.is_static())
            result = m_method.invoke({}, args);
        else
            m_method.invoke_variadic(object, args);

        if (!result.is_type<void>())
            return Result(serialization::RttrConverter::toJson(result));

        return Result();
    }


    // void MethodNode::resolve()
    // {
    //     if (!isResolved())
    //     {
    //         if (auto pos = m_name.find("::"); pos != std::string::npos)
    //         {
    //             const std::string className  = m_name.substr(0, pos);
    //             const std::string methodName = m_name.substr(pos + 2);
    //
    //             m_object = rttr::type::get_by_name(className);
    //             if (m_object.value().is_valid())
    //                 m_method = m_object.value().get_method(methodName);
    //         }
    //     }
    //
    //     m_resolved = true;
    // }
    //
    // Result MethodNode::invoke()
    // {
    //         auto pos = m_name.find("::");
    //         if (pos == std::string::npos)
    //             throw ExecutionError(ErrorType::MissingNode, "Invalid method name: " + m_name);
    //
    //         const std::string className = m_name.substr(0, pos);
    //         rttr::variant instance;
    //         if (className != "GraphIO")
    //             instance = ObjectStore::getInstance().retrieveVariant(className);
    //
    //         rttr::variant result;
    //         if (m_method->is_static())
    //         {
    //             result = m_method.value().invoke({}, args[0], args[1], nullptr);
    //         }
} // namespace engine::operation