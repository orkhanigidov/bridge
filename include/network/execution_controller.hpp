#pragma once

#include "dto/execute_request_dto.hpp"
#include "dto/execute_response_dto.hpp"
#include "dto/message_dto.hpp"
#include <oatpp/codegen/ApiController_define.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace network {
    class ExecutionController final
        : public oatpp::web::server::api::ApiController {
    public:
        explicit ExecutionController(
                OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, object_mapper))
            : ApiController(std::move(object_mapper))
        {}

        ENDPOINT_INFO(health_check)
        {
            info->summary = "Health check";
            info->description = "Returns 200 OK when the service is ready to "
                                "accept requests";
            info->addResponse<oatpp::Object<dto::MessageDto>>(
                    Status::CODE_200, "application/json");
        }
        ENDPOINT("GET", "/health", health_check)
        {
            const auto msg = dto::MessageDto::createShared();
            msg->message = "OK";
            return createDtoResponse(Status::CODE_200, msg);
        }

        ENDPOINT_INFO(execute_script)
        {
            info->summary = "Executes a Lua script";
            info->description
                    = "Accepts an execution request and returns the result";
            info->addConsumes<oatpp::Object<dto::ExecuteRequestDto>>(
                    "application/json");
            info->addResponse<oatpp::Object<dto::ExecuteResponseDto>>(
                    Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::MessageDto>>(
                    Status::CODE_400, "application/json");
        }
        ENDPOINT("POST", "/execute", execute_script,
                 BODY_DTO(oatpp::Object<dto::ExecuteRequestDto>, request))
        {
            if (!request) {
                const auto err = dto::MessageDto::createShared();
                err->message = "Invalid request body";
                return createDtoResponse(Status::CODE_400, err);
            }
            const auto response = dto::ExecuteResponseDto::createShared();
            return createDtoResponse(Status::CODE_200, response);
        }
    };
} // namespace network

#include OATPP_CODEGEN_END(ApiController)
