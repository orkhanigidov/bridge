#include "../../include/pch.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/ObjectNode.h"

namespace engine::operation
{
    MethodNode::MethodNode(const std::string& id, const std::string& name): InvokableNode(id, NodeType::Method, name)
    {
    }

    rttr::method MethodNode::getMethod() const
    {
        if (isResolved()) return m_method.value();
        throw ExecutionError(ErrorType::MissingNode, "Method not found: " + m_name);
    }

    rttr::type MethodNode::getObject() const
    {
        if (isResolved()) return m_object.value();
        throw ExecutionError(ErrorType::MissingNode, "Object not found: " + m_name);
    }

    void MethodNode::resolve()
    {
        if (!isResolved())
        {
            if (auto pos = m_name.find("::"); pos != std::string::npos)
            {
                const std::string className = m_name.substr(0, pos);
                const std::string methodName = m_name.substr(pos + 2);

                m_object = rttr::type::get_by_name(className);
                if (m_object.value().is_valid()) m_method = m_object.value().get_method(methodName);
            }
        }

        m_resolved = true;
    }

    Result MethodNode::invoke()
    {
        try
        {
            resolve();
            if (!isValid()) throw ExecutionError(ErrorType::MissingNode, "Method not found: " + m_name);

            const rttr::instance instance = m_object.value();
            const std::vector<rttr::argument> args = prepareArguments(m_method.value());
            const rttr::variant result = m_method.value().invoke_variadic(instance, args);

            return Result(result);
        }
        catch (const std::exception& e)
        {
            throw ExecutionError(ErrorType::FailedExecution, "Failed to execute method: " + m_name + ": " + e.what());
        }
    }
} // namespace engine::operation
