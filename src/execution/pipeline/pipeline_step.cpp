#define SOL_ALL_SAFETIES_ON 1

#include "engine/dto/pipeline_step.hpp"

#include "engine/execution/pipeline/pipeline_step.hpp"
#include "engine/lua_bindings/lua_registrar.hpp"
#include "engine/serialization/oatpp_type_adapter.hpp"
#include "pch.hpp"

namespace engine::pipeline
{
    pipeline_step::pipeline_step(const dto::pipeline_step& dto)
    {
        if (dto.class_name)
        {
            name_ = *dto.class_name;
        }
        else if (dto.function_name)
        {
            name_ = *dto.function_name;
        }
        else
        {
            throw std::invalid_argument(
                "Pipeline step must have either 'class_name' or 'function_name'");
        }

        if (dto.alias)
        {
            alias_ = *dto.alias;
        }

        if (dto.constructor_args)
        {
            parameters_.reserve(dto.constructor_args->size());
            for (const auto& arg : *dto.constructor_args)
            {
                parameters_.emplace_back(serialization::oatpp_type_adapter::from_oatpp(arg));
            }
        }

        if (dto.function_args)
        {
            parameters_.reserve(dto.function_args->size());
            for (const auto& arg : *dto.function_args)
            {
                parameters_.emplace_back(serialization::oatpp_type_adapter::from_oatpp(arg));
            }
        }
    }

    void pipeline_step::execute() const
    {
        if (is_constructor())
        {
            execute_constructor();
        }
        else if (is_member_function())
        {
            execute_member_function();
        }
        else
        {
            execute_global_function();
        }
    }

    void pipeline_step::execute_constructor() const
    {
        auto& lua = lua_bindings::lua_registrar::instance().get_lua();
        lua.safe_script(R"(object_pool = object_pool or {})");

        try
        {
            lua["object_pool"][*alias_] =
                parameters_.empty() ? lua[name_]["new"]()
                                    : lua[name_]["new"](sol::as_args(resolve_parameters(lua)));
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Failed to create instance of " + name_ + ": " + e.what());
        }
    }


    void pipeline_step::execute_member_function() const
    {
        auto& lua = lua_bindings::lua_registrar::instance().get_lua();

        const auto dot_pos = name_.find('.');
        if (dot_pos == std::string::npos)
        {
            throw std::invalid_argument("Invalid member function format: " + name_);
        }


        const auto instance_name = name_.substr(0, dot_pos);
        auto method_name         = name_.substr(dot_pos + 1);
        auto clean_instance_name =
            instance_name[0] == '$' ? instance_name.substr(1) : instance_name;

        try
        {
            const sol::function func = lua["object_pool"][clean_instance_name][method_name];
            const sol::protected_function_result result =
                parameters_.empty() ? func(lua["object_pool"][clean_instance_name])
                                    : func(lua["object_pool"][clean_instance_name],
                                           sol::as_args(resolve_parameters(lua)));

            if (!result.valid())
            {
                throw std::runtime_error(name_ + ": " + sol::error(result).what());
            }
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Member function call failed: " + name_ + ": " + e.what());
        }
    }

    void pipeline_step::execute_global_function() const
    {
        auto& lua = lua_bindings::lua_registrar::instance().get_lua();

        try
        {
            const sol::protected_function_result result =
                parameters_.empty() ? lua[name_]()
                                    : lua[name_](sol::as_args(resolve_parameters(lua)));

            if (!result.valid())
            {
                throw std::runtime_error(name_ + ": " + sol::error(result).what());
            }
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Global function call failed: " + name_ + ": " + e.what());
        }
    }

    std::vector<sol::object> pipeline_step::resolve_parameters(sol::state& lua) const
    {
        std::vector<sol::object> resolved_args;
        resolved_args.reserve(parameters_.size());

        for (const auto& parameter : parameters_)
        {
            if (std::holds_alternative<std::string>(parameter))
            {
                auto str = std::get<std::string>(parameter);
                if (!str.empty() && str[0] == '$')
                {
                    auto var_name = str.substr(1);
                    if (lua["object_pool"].valid() && lua["object_pool"][var_name].valid())
                    {
                        resolved_args.emplace_back(
                            sol::make_object(lua, lua["object_pool"][var_name]));
                    }
                    else
                    {
                        throw std::runtime_error("Variable '" + var_name +
                                                 "' not found in object pool");
                    }
                    continue;
                }
            }
            std::visit([&](auto&& value)
                       { resolved_args.emplace_back(sol::make_object(lua, value)); }, parameter);
        }
        return resolved_args;
    }

    bool pipeline_step::is_constructor() const noexcept
    {
        return !name_.empty() && std::isupper(name_[0]);
    }

    bool pipeline_step::is_member_function() const noexcept
    {
        return name_.find('.') != std::string::npos;
    }
} // namespace engine::pipeline