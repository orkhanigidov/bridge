#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_dto.hpp"
#include "file_dto.hpp"
#include "metadata_dto.hpp"
#include "status_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    class ResponseDto final : public oatpp::DTO
    {
        DTO_INIT(ResponseDto, DTO)

        DTO_FIELD(oatpp::Enum<ExecutionStatusDto>::AsString, status);
        DTO_FIELD(oatpp::List<oatpp::Object<FileDto>>, output_data);
        DTO_FIELD(oatpp::Object<ErrorDto>, error);
        DTO_FIELD(oatpp::Object<MetadataDto>, metadata);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
