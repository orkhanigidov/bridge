#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include "error_dto.hpp"
#include "file_dto.hpp"
#include "metadata_dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace network::dto::execution {
    
    ENUM(ExecutionStatus, v_int8,
        VALUE(SUCCESS, 0, "success"),
        VALUE(FAILURE, 1, "failure"),
        VALUE(TIMEOUT, 2, "timeout"),
        VALUE(UNKNOWN, 99, "unknown")
    )

    class ResponseDto final : public oatpp::DTO {
        DTO_INIT(ResponseDto, DTO)

        DTO_FIELD(Enum<ExecutionStatus>::AsString, status);
        DTO_FIELD(Object<FileDto>, output_data);
        DTO_FIELD(Object<ErrorDto>, error);
        DTO_FIELD(Object<MetadataDto>, metadata);
    };

} // namespace network::dto::execution

#include OATPP_CODEGEN_END(DTO)
