#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution {

    class MetadataDto final : public oatpp::DTO {
        DTO_INIT(MetadataDto, DTO)

        DTO_FIELD(UInt64, duration_ms);
    };

} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
