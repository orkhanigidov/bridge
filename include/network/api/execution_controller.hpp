/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file execution_controller.hpp
 * @brief Declares the ExecutionController API controller for execution-related endpoints.
 */

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
    /**
     * @class ExecutionController
     * @brief API controller for handling execution-related HTTP requests.
     */
    class ExecutionController final : public oatpp::web::server::api::ApiController
    {
    public:
        /**
         * @brief Constructor.
         * @param object_mapper The object mapper for DTO serialization.
         */
        explicit ExecutionController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& object_mapper)
            : ApiController(object_mapper)
        {
        }

        /**
         * @brief Endpoint info for health check.
         */
        ENDPOINT_INFO(health)
        {
            info->summary = "Check service health status";
            info->addResponse<Object<dto::MessageDto>>(Status::CODE_200, "application/json");
        }

        /**
         * @brief Health check endpoint.
         * @return 200 OK with a status message.
         */
        ENDPOINT("GET", "/api/health", health)
        {
            const auto response = dto::MessageDto::createShared();
            response->status_code = 200;
            response->message = "OK";
            return createDtoResponse(Status::CODE_200, response);
        }

        /**
         * @brief Endpoint info for script or pipeline execution.
         */
        ENDPOINT_INFO(execution)
        {
            info->summary = "Execute a script or pipeline";
            info->addConsumes<oatpp::Object<dto::execution::RequestDto>>("application/json");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_200, "application/json", "Execution was successful");
            info->addResponse<oatpp::Object<dto::execution::ResponseDto>>(Status::CODE_500, "application/json", "Execution failed");
            info->addResponse<oatpp::Object<dto::MessageDto>>(Status::CODE_400, "application/json", "Invalid request body");
        }

        /**
         * @brief Endpoint for executing a script or pipeline.
         * @param request The execution request DTO.
         * @return 200 OK with an execution result, 400 for invalid request, or 500 for errors.
         */
        ENDPOINT("POST", "/api/execute_script", execution, BODY_DTO(oatpp::Object<dto::execution::RequestDto>, request))
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
