#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace engine::dto
{
#include OATPP_CODEGEN_BEGIN(DTO)

    class EngineDTO final : public oatpp::DTO
    {
        DTO_INIT(EngineDTO, DTO)
    };

#include OATPP_CODEGEN_END(DTO)
} // namespace engine::dto