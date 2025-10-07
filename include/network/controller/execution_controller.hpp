#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/response_dto.hpp"
#include "network/dto/message_dto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace engine::network::controller
{
    class ExecutionController final : public oatpp::web::server::api::ApiController
    {
    public:
        explicit ExecutionController(OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper)):
            ApiController(object_mapper)
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
            info->addResponse<Object<dto::execution::ResponseDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_400, "application/json");
        }

        ENDPOINT("POST", "/execution", execution, BODY_DTO(Object<dto::execution::RequestDto>, request))
        {
            if (!request)
            {
                auto error = dto::MessageDto::createShared();
                error->status_code = 400;
                error->message = "Request body is missing or malformed";
                return createDtoResponse(Status::CODE_400, error);
            }
            auto response = dto::execution::ResponseDto::createShared();
            response->status = dto::execution::ExecutionStatus::SUCCESS;
            return createDtoResponse(Status::CODE_200, response);
        }
    };
} // namespace engine::network::controller

#include OATPP_CODEGEN_END(ApiController)
