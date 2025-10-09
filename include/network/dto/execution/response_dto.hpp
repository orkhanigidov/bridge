#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_dto.hpp"
#include "file_dto.hpp"
#include "metadata_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    ENUM(ExecutionStatusDto, v_int8,
         VALUE(SUCCESS, 0, "Success"),
         VALUE(FAILURE, 1, "Failure"),
         VALUE(TIMEOUT, 2, "Timeout")
    )

    class ResponseDto final : public oatpp::DTO
    {
        DTO_INIT(ResponseDto, DTO)

        DTO_FIELD(Enum<ExecutionStatusDto>::AsString, status);
        DTO_FIELD(Object<FileDto>, output_data);
        DTO_FIELD(Object<ErrorDto>, error);
        DTO_FIELD(Object<MetadataDto>, metadata);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
