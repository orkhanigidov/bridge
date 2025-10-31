/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */
#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_type_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    class ErrorDto final : public oatpp::DTO
    {
        DTO_INIT(ErrorDto, DTO)

        DTO_FIELD(oatpp::Enum<ExecutionErrorTypeDto>::AsString, type);
        DTO_FIELD(oatpp::String, message);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
