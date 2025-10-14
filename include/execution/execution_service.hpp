#pragma once

#include "execution_result.hpp"
#include "network/dto/execution/request_dto.hpp"

#include <oatpp/core/Types.hpp>

namespace engine::execution
{
    class ExecutionService
    {
    public:
        ExecutionResult execute(const oatpp::Object<network::dto::execution::RequestDto>& request);

    private:
        std::string prepare_script(const std::string& script, const fs::path& input_path, const fs::path& output_path);
    };
}
