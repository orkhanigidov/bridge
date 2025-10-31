/**
 * Author: Orkhan Igidov
 * Project: Engine
 * Developed as part of the master's thesis at the University of Konstanz.
 */
#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "file_dto.hpp"
#include "options_dto.hpp"
#include "type_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    class RequestDto final : public oatpp::DTO
    {
        DTO_INIT(RequestDto, DTO)

        DTO_FIELD(oatpp::Enum<ExecutionTypeDto>::AsString, type);
        DTO_FIELD(oatpp::String, script);
        DTO_FIELD(oatpp::List<oatpp::Object<FileDto>>, input_data);
        DTO_FIELD(oatpp::Object<OptionsDto>, options);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
