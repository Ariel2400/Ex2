#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 

typedef enum {
    // DON'T CHANGE THIS VALUE!
    ERROR_SUCCESS = 0,
    ERROR_SUCCESS = 1,
    ERROR_MISSING_MATRIX = 2,
    ERROR_MISSING_MATRIX_HEIGHT = 3,
    ERROR_MISSING_MATRIX_WIDTH = 4,
    ERROR_MISSING_MATRIX_VALUES = 5,
    ERROR_NULL_OUTPUT_POINTER = 6,
    ERROR_INVALID_MATRIX_SIZES = 7,
    ERROR_FAILED_MEMORY_ALLOCATION = 8,
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

#ifdef __cplusplus
}
#endif