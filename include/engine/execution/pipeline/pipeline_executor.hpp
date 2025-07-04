#pragma once

#include "engine/dto/pipeline.hpp"

namespace engine::pipeline
{
    class pipeline_executor final
    {
      public:
        void execute(const dto::pipeline& pipeline) const;

      private:
        void execute_pipeline_steps(const std::list<oatpp::data::mapping::type::DTOWrapper<dto::pipeline_step>>& steps) const;
    };
} // namespace engine::pipeline