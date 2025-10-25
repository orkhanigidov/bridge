#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace engine::network::dto::execution
{
    class FileDto final : public oatpp::DTO
    {
        DTO_INIT(FileDto, DTO)

        DTO_FIELD(oatpp::String, id);
        DTO_FIELD(oatpp::UInt16, chunk_index);
        DTO_FIELD(oatpp::UInt16, total_chunks);
        DTO_FIELD(oatpp::String, chunk_data);
    };
} // namespace engine::network::dto::execution

#include OATPP_CODEGEN_END(DTO)
