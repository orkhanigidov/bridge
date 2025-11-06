/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */

/**
 * @file request_mapper.hpp
 * @brief Declares the RequestMapper utility for mapping execution requests between internal and DTO representations.
 */

#pragma once

#include <oatpp/core/Types.hpp>

#include "options_mapper.hpp"
#include "interop/types/execution_request.h"
#include "interop/types/execution_type.h"
#include "network/dto/execution/file_dto.hpp"
#include "network/dto/execution/request_dto.hpp"
#include "network/dto/execution/type_dto.hpp"

namespace engine::network::mapper
{
    /**
     * @class RequestMapper
     * @brief Utility class for mapping execution requests between internal and DTO representations.
     */
    class RequestMapper final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        RequestMapper() = delete;

        /**
         * @brief Convertss internal ExecutionType to a DTO enum.
         * @param type The internal execution type.
         * @return The corresponding ExecutionTypeDto enum.
         */
        static oatpp::Enum<dto::execution::ExecutionTypeDto> to_dto(const interop::types::ExecutionType& type)
        {
            return static_cast<dto::execution::ExecutionTypeDto>(type);
        }

        /**
         * @brief Converts internal ExecutionRequest and input data to a RequestDto object.
         * @param request The internal execution request.
         * @param input_data The input file data as a FileDto object.
         * @return The corresponding RequestDto object.
         */
        static oatpp::Object<dto::execution::RequestDto> to_dto(const interop::types::ExecutionRequest& request,
                                                                const oatpp::Object<dto::execution::FileDto>& input_data)
        {
            auto dto = dto::execution::RequestDto::createShared();
            dto->type = to_dto(request.type);
            dto->script = request.script;
            dto->input_data = input_data;
            dto->options = OptionsMapper::to_dto(request.options);
            return dto;
        }
    };
} // namespace engine::network::mapper
