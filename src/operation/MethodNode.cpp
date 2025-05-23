#include "../../include/pch.h"
#include "../../include/operation/MethodNode.h"
#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/ObjectNode.h"
#include "../../include/serialization/JsonRttrConverter.h"
#include "../../include/operation/PersistentObjectStore.h"

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
            if (!isResolved()) resolve();
            if (!isValid()) throw ExecutionError(ErrorType::MissingNode, "Method not found: " + m_name);

            auto pos = m_name.find("::");
            if (pos == std::string::npos)
                throw ExecutionError(ErrorType::MissingNode, "Invalid method name: " + m_name);

            const std::string className = m_name.substr(0, pos);
            rttr::variant instance;
            if (className != "GraphIO")
                instance = PersistentObjectStore::getInstance().retrieveVariant(className);

            const std::vector<rttr::variant> params = prepareArguments(m_method.value());
            const std::vector<rttr::argument> args(params.begin(), params.end());

            rttr::variant result;
            if (m_method->is_static()) {
                result = m_method.value().invoke({}, args[0], args[1], nullptr);
            } else {
                result = m_method.value().invoke_variadic(instance, args);
            }

            if (!result.is_type<void>()) {
                std::cout << serialization::JsonRttrConverter::variantToJson(result) << std::endl;
            }

            return Result(result);
        }
        catch (const std::exception& e)
        {
            throw ExecutionError(ErrorType::FailedExecution, "Failed to execute method: " + m_name + ": " + e.what());
        }
    }
} // namespace engine::operation
