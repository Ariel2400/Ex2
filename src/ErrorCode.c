#include "ErrorCode.h"

bool error_isSuccess(ErrorCode code) {
    return code == ERROR_SUCCESS;
}

const char* error_getErrorMessage(ErrorCode code) {
    switch(code) {
        case ERROR_SUCCESS: return "The operation was performed successfully";
        case ERROR_MISSING_MATRIX: return "Error: input matrix pointer equals NULL";
        case ERROR_MISSING_MATRIX_HEIGHT: return "Error: input matrix height missing";
        case ERROR_MISSING_MATRIX_WIDTH: return "Error: input matrix width missing";
        case ERROR_MISSING_MATRIX_VALUES: return "Error: values missing from input matrix";
        case ERROR_NULL_OUTPUT_POINTER: return "Error: tried to store information into a NULL adress";
        case ERROR_INVALID_MATRIX_SIZES: return "Error: tried to perform an operation on matrices with invalid sizes";
        case ERROR_FAILED_MEMORY_ALLOCATION: return "Error: failed to allocate memory";
        default: return "";
    }
}