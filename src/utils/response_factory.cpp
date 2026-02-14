/**
 * Project: Engine
 * File: response_factory.cpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file response_factory.cpp
 * @brief Implements the ResponseFactory utility for creating execution response.
 */

#include "utils/response_factory.hpp"

#include <cstdlib>
#include <cstring>
#include <string>

#include "interop/types/execution_error_type.h"
#include "interop/types/execution_metadata.h"
#include "interop/types/execution_response.h"
#include "interop/types/execution_status.h"

#if defined(_WIN32)
    #define strdup _strdup
#endif

namespace engine::utils
{
    /**
     * @brief Custom deleter for ExecutionResponse pointers. Frees the error message and deletes the response object.
     * @param ptr Pointer to ExecutionResponse.
     */
    void ExecutionResponseDeleter::operator()(interop::types::ExecutionResponse* ptr) const
    {
        if (!ptr)
        {
            return;
        }

        if (ptr->error.message)
        {
            free(ptr->error.message);
        }

        if (ptr->string_output)
        {
            free(ptr->string_output);
        }

        delete ptr;
    }

    /**
     * @brief Creates an error execution response.
     * @param status The execution status.
     * @param type The error type.
     * @param message The error message.
     * @return Unique pointer to the created ExecutionResponse.
     */
    ExecutionResponsePtr ResponseFactory::create_error(interop::types::ExecutionStatus status,
                                                       interop::types::ExecutionErrorType type,
                                                       const std::string& message)
    {
        ExecutionResponsePtr response(new interop::types::ExecutionResponse(), ExecutionResponseDeleter());

        response->status = status;
        response->error.type = type;
        response->error.message = strdup(message.c_str());

        return response;
    }

    /**
     * @brief Creates a success execution repose.
     * @param metadata The execution metadata.
     * @param string_output Optional string output from the script.
     * @return Unique pointer to the created ExecutionResponse.
     */
    ExecutionResponsePtr ResponseFactory::create_success(interop::types::ExecutionMetadata metadata,
                                                         const std::string& string_output)
    {
        ExecutionResponsePtr response(new interop::types::ExecutionResponse(), ExecutionResponseDeleter());

        response->status = interop::types::ExecutionStatus::Success;
        response->metadata = metadata;

        response->error.type = {};
        response->error.message = nullptr;

        if (!string_output.empty())
        {
            response->string_output = strdup(string_output.c_str());
        }
        else
        {
            response->string_output = nullptr;
        }

        return response;
    }
} // namespace engine::utils
