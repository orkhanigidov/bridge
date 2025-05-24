#include "../../include/pch.h"
#include "../../include/operation/FunctionNode.h"
#include "../../include/operation/ExecutionError.h"
#include "../../include/operation/Result.h"
#include "../../include/operation/PersistentObjectStore.h"
#include "../../include/serialization/JsonRttrConverter.h"

namespace engine::operation
{
    FunctionNode::FunctionNode(const std::string& id, const std::string& name)
        : InvokableNode(id, NodeType::Function, name)
    {
    }

    rttr::method FunctionNode::getMethod() const
    {
        if (isResolved()) return m_method.value();
        throw ExecutionError(ErrorType::MissingNode, "Function not found: " + m_name);
    }

    void FunctionNode::resolve()
    {
        if (!isResolved())
        {
            m_method = rttr::type::get_global_method(m_name);
            m_resolved = true;
        }
    }

    Result FunctionNode::invoke()
    {
        try
        {
            resolve();
            if (!isValid()) throw ExecutionError(ErrorType::MissingNode, "Function not found: " + m_name);

            rttr::variant& originalG = PersistentObjectStore::getInstance().retrieveVariant("G");

            const std::vector<rttr::variant> params = prepareArguments(m_method.value());
            const std::vector<rttr::argument> args(params.begin(), params.end());
            const rttr::variant result = m_method.value().invoke_variadic({}, args);

            for (const auto& param : params) {
                if (param.get_type() == originalG.get_type()) {
                    PersistentObjectStore::getInstance().updateVariant("G", param);
                }
            }

            if (!result.is_type<void>()) {
                std::cout << serialization::JsonRttrConverter::variantToJson(result) << std::endl;
            }

            return Result(result);
        }
        catch (const std::exception& e)
        {
            throw ExecutionError(ErrorType::FailedExecution, "Failed to execute function: " + m_name + ": " + e.what());
        }
    }
} // namespace engine::operation
