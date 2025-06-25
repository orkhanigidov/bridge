#define SOL_ALL_SAFETIES_ON 1

#include "pipeline/Pipeline.hpp"

#include "dto/PipelineDTO.hpp"
#include "pch.hpp"
#include "registration/LuaRegistrar.hpp"

namespace engine::pipeline
{
    Pipeline::Pipeline(const dto::PipelineDTO& dto)
    {
        if (!dto.name)
        {
            throw std::invalid_argument("Pipeline DTO must contain 'pipeline' field");
        }

        name_ = *dto.name;

        if (dto.steps)
        {
            commands_.reserve(dto.steps->size());

            for (const auto& step : *dto.steps)
            {
                commands_.emplace_back(std::make_unique<Command>(*step.get()));
            }
        }
    }

    void Pipeline::execute() const
    {
        auto& lua = registration::LuaRegistrar::instance().get_lua();

        lua.safe_script(R"(object_pool = object_pool or {})", sol::script_pass_on_error);

        for (auto& command : commands_)
        {
            command->execute();
        }
    }
} // namespace engine::pipeline