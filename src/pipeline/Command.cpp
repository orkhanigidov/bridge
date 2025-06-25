#define SOL_ALL_SAFETIES_ON 1

#include "pipeline/Command.hpp"

#include "dto/StepDTO.hpp"
#include "pch.hpp"
#include "registration/LuaRegistrar.hpp"
#include "serialization/Converter.hpp"

namespace engine::pipeline
{
    Command::Command(const dto::StepDTO& dto)
    {
        if (dto.obj)
        {
            name_ = *dto.obj;
        }
        else if (dto.func)
        {
            name_ = *dto.func;
        }
        else
        {
            throw std::invalid_argument("Command must contain either 'obj' or 'func'");
        }

        if (dto.alias)
        {
            alias_ = *dto.alias;
        }

        if (dto.params)
        {
            parameters_.reserve(dto.params->size());
            for (const auto& param : *dto.params)
            {
                parameters_.emplace_back(serialization::Converter::to_cpp_value(param));
            }
        }
    }

    void Command::execute() const
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

    void Command::execute_constructor() const
    {
        auto& lua = registration::LuaRegistrar::instance().get_lua();

        if (!alias_)
        {
            throw std::invalid_argument("Constructor must have 'alias' value");
        }

        try
        {
            lua["object_pool"][*alias_] = parameters_.empty()
                                            ? lua[name_]["new"]()
                                            : lua[name_]["new"](sol::as_args(resolve_params(lua)));
        }
        catch (const sol::error& e)
        {
            throw std::runtime_error("Failed to create instance of " + name_ + ": " + e.what());
        }
    }


    void Command::execute_member_function() const
    {
        auto& lua = registration::LuaRegistrar::instance().get_lua();

        auto dot_pos        = name_.find('.');
        auto instance_name  = name_.substr(0, dot_pos);
        auto method_name    = name_.substr(dot_pos + 1);
        auto clean_instance = instance_name[0] == '$' ? instance_name.substr(1) : instance_name;

        try
        {
            sol::function func = lua["object_pool"][clean_instance][method_name];
            sol::protected_function_result result =
                parameters_.empty()
                    ? func(lua["object_pool"][clean_instance])
                    : func(lua["object_pool"][clean_instance], sol::as_args(resolve_params(lua)));

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

    void Command::execute_global_function() const
    {
        auto& lua = registration::LuaRegistrar::instance().get_lua();

        try
        {
            sol::protected_function_result result =
                parameters_.empty() ? lua[name_]() : lua[name_](sol::as_args(resolve_params(lua)));

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

    std::vector<sol::object> Command::resolve_params(sol::state& lua) const
    {
        std::vector<sol::object> args;
        for (const auto& param : parameters_)
        {
            if (std::holds_alternative<std::string>(param))
            {
                auto str = std::get<std::string>(param);
                if (!str.empty() && str[0] == '$')
                {
                    auto var_name = str.substr(1);
                    if (lua["object_pool"].valid() && lua["object_pool"][var_name].valid())
                    {
                        args.emplace_back(sol::make_object(lua, lua["object_pool"][var_name]));
                    }
                    else
                    {
                        throw std::runtime_error("Variable '" + var_name +
                                                 "' not found in object pool");
                    }
                    continue;
                }
            }
            std::visit([&](auto&& value) { args.emplace_back(sol::make_object(lua, value)); },
                       param);
        }
        return args;
    }

    bool Command::is_constructor() const noexcept
    {
        return !name_.empty() && std::isupper(name_[0]);
    }

    bool Command::is_member_function() const noexcept
    {
        return name_.find('.') != std::string::npos;
    }
} // namespace engine::pipeline