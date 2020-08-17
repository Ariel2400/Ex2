#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    // DON'T CHANGE THIS VALUE!
    ERROR_SUCCESS = 0,
    ERROR_MISSING_MATRIX = 1,
    ERROR_MISSING_MATRIX_HEIGHT = 2,
    ERROR_MISSING_MATRIX_WIDTH = 3,
    ERROR_MISSING_MATRIX_VALUES = 4,
    ERROR_NULL_OUTPUT_POINTER = 5,
    ERROR_INVALID_MATRIX_SIZES = 6,
    ERROR_FAILED_MEMORY_ALLOCATION = 7
} ErrorCode;

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code);

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char* error_getErrorMessage(ErrorCode code);