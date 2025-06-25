#pragma once

#include "Command.hpp"
#include "dto/PipelineDTO.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    class Pipeline
    {
      public:
        explicit Pipeline(const dto::PipelineDTO& dto);

        void execute() const;

      private:
        std::string name_;
        std::vector<std::unique_ptr<Command>> commands_;
    };
} // namespace engine::pipeline