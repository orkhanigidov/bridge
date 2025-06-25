#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class NetworkDTO final : public oatpp::DTO
    {
        DTO_INIT(NetworkDTO, DTO)
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto