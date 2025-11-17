/**
 * Project: Engine
 * File: interop_c_api.hpp
 *
 * Copyright (C) 2025 Orkhan Igidov
 * Developed as part of the master's thesis at the University of Konstanz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file interop_c_api.hpp
 * @brief Declares the C API for the engine interop layer.
 */

#pragma once

#include "engine_export.hpp"
#include "types/execution_request.h"
#include "types/execution_response.h"

/**
 * @typedef ExecutionRequest
 * @brief Alias for engine::interop::types::ExecutionRequest for C API usage.
 */
typedef engine::interop::types::ExecutionRequest ExecutionRequest;

/**
 * @typedef ExecutionResponse
 * @brief Alias for engine::interop::types::ExecutionResponse for C APU usage.
 */
typedef engine::interop::types::ExecutionResponse ExecutionResponse;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the engine bindings for interop.
 * @return True if initialization was successful, false otherwise.
 */
ENGINE_API bool InitializeBindings();

/**
 * @brief Executes a script or pipeline using the provider request.
 * @param request Pointer to the execution request.
 * @return Pointer to the execution response. The caller is responsible for freeing it with FreeExecutionResponse.
 */
ENGINE_API ExecutionResponse* ExecuteScript(const ExecutionRequest* request);

/**
 * @brief Frees the memory allocated for an ExecutionResponse.
 * @param response Pointer to the execution response to free.
 */
ENGINE_API void FreeExecutionResponse(ExecutionResponse* response);

#ifdef __cplusplus
} // extern "C"
#endif
