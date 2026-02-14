/**
 * Project: Engine
 * File: response_factory.hpp
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
 * @file response_factory.hpp
 * @brief Declares the ResponseFactory utility for creating execution responses.
 */

#pragma once

#include <memory>
#include <string>

#include "interop/types/execution_error_type.h"
#include "interop/types/execution_metadata.h"
#include "interop/types/execution_response.h"
#include "interop/types/execution_status.h"

namespace engine::utils
{
    /**
     * @struct ExecutionResponseDeleter
     * @brief Custom deleter for ExecutionResponse pointers.
     */
    struct ExecutionResponseDeleter
    {
        void operator()(interop::types::ExecutionResponse* ptr) const;
    };

    /**
     * @typedef ExecutionResponsePtr
     * @brief Unique pointer type for ExecutionResponse with custom deleter.
     */
    using ExecutionResponsePtr = std::unique_ptr<interop::types::ExecutionResponse, ExecutionResponseDeleter>;

    /**
     * @class ResponseFactory
     * @brief Unique pointer type for ExecutionResponse with custom deleter.
     */
    class ResponseFactory final
    {
    public:
        /**
         * @brief Deleted default constructor to prevent instantiation.
         */
        ResponseFactory() = delete;

        /**
         * @brief Creates an error execution response.
         * @param status The execution status.
         * @param type The error type.
         * @param message The error message.
         * @return Unique pointer to the created ExecutionResponse.
         */
        static ExecutionResponsePtr create_error(interop::types::ExecutionStatus status,
                                                 interop::types::ExecutionErrorType type,
                                                 const std::string& message);

        /**
         * @brief Creates a success execution responses.
         * @param metadata The execution metadata.
         * @param string_output Optional string output from the script.
         * @return Unique pointer to the created ExecutionResponse.
         */
        static ExecutionResponsePtr create_success(interop::types::ExecutionMetadata metadata,
                                                   const std::string& string_output = "");
    };
} // namespace engine::utils
