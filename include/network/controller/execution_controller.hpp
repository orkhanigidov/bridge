#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "execution/execution_service.hpp"
#include "execution/script/script_executor.hpp"
#include "mapper/execution_result_mapper.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/dto/message_dto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace engine::network::controller
{
    class ExecutionController final : public oatpp::web::server::api::ApiController
    {
    public:
        explicit ExecutionController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& object_mapper,
                                     std::shared_ptr<execution::ExecutionService> execution_service)
            : ApiController(object_mapper), execution_service_(std::move(execution_service))
        {
        }

        ENDPOINT_INFO(health)
        {
            info->summary = "Check service health status";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json");
        }

        ENDPOINT("GET", "/health", health)
        {
            auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = "OK";
            return createDtoResponse(Status::CODE_200, response);
        }

        ENDPOINT_INFO(execution)
        {
            info->summary = "Execute a script or pipeline";
            info->addConsumes<Object<dto::execution::RequestDto>>("application/json");
            info->addResponse<Object<dto::execution::ResponseDto>>(Status::CODE_200, "application/json",
                                                                   "Execution was successful");
            info->addResponse<Object<dto::execution::ResponseDto>>(Status::CODE_500, "application/json",
                                                                   "Execution failed");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_400, "application/json", "Invalid request body");
        }

        ENDPOINT("POST", "/execute_script", execution, BODY_DTO(Object<dto::execution::RequestDto>, request))
        {
            if (!request)
            {
                auto error = dto::MessageDto::createShared();
                error->status_code = 400;
                error->message = "Request body is missing or malformed";
                return createDtoResponse(Status::CODE_400, error);
            }

            try
            {
                auto result = execution_service_->execute(request);
                auto response_dto = mapper::ExecutionResultMapper::to_dto(result);
                auto status = result.is_success ? Status::CODE_200 : Status::CODE_500;
                return createDtoResponse(status, response_dto);
            }
            catch (const std::exception& e)
            {
                auto error = dto::MessageDto::createShared();
                error->status_code = 500;
                error->message = "An unexpected error occurred: " + std::string(e.what());
                return createDtoResponse(Status::CODE_500, error);
            }
        }

    private:
        std::shared_ptr<execution::ExecutionService> execution_service_;
    };
} // namespace engine::network::controller

#include OATPP_CODEGEN_END(ApiController)
