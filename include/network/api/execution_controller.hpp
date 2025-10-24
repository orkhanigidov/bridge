#pragma once

#include <exception>
#include <format>
#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "execution/core_execution_result.hpp"
#include "execution/execution_service.hpp"
#include "network/dto/message_dto.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/mapper/execution_result_mapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace engine::network::api
{
    class ExecutionController final : public oatpp::web::server::api::ApiController
    {
    public:
        explicit ExecutionController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& object_mapper)
            : ApiController(object_mapper)
        {
        }

        ENDPOINT_INFO(health)
        {
            info->summary = "Check service health status";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json");
        }

        ENDPOINT("GET", "/health", health)
        {
            const auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = "OK";
            return createDtoResponse(Status::CODE_200, response);
        }

        ENDPOINT_INFO(execution)
        {
            info->summary = "Execute a script or pipeline";
            info->addConsumes<oatpp::Object<dto::execution::RequestDto>>("application/json");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_200, "application/json", "Execution was successful");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_500, "application/json", "Execution failed");
            info->addResponse<oatpp::Object<dto::MessageDto>>(Status::CODE_400, "application/json", "Invalid request body");
        }

        ENDPOINT("POST", "/execute_script", execution, BODY_DTO(oatpp::Object<dto::execution::RequestDto>, request))
        {
            if (!request)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 400;
                error->message = "Request body is missing or malformed";
                return createDtoResponse(Status::CODE_400, error);
            }

            try
            {
                const auto result = execution::ExecutionService::execute(request);
                const auto response_dto = mapper::ExecutionResultMapper::to_dto(result);
                const auto status = result.is_success() ? Status::CODE_200 : Status::CODE_500;
                return createDtoResponse(status, response_dto);
            } catch (const std::exception& e)
            {
                const auto error = dto::MessageDto::createShared();
                error->status_code = 500;
                error->message = std::format("An unexpected error occurred: {}", e.what());
                return createDtoResponse(Status::CODE_500, error);
            }
        }
    };
} // namespace engine::network::api

#include OATPP_CODEGEN_END(ApiController)
