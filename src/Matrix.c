#include <stddef.h>
#include <stdlib.h>

#include "Matrix.h"
#include "ErrorCode.h"

struct Matrix {
    uint32_t* height;
    uint32_t* width;
    double** array;
};

ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    if (matrix == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    PMatrix newMatrixPointer = (PMatrix)malloc(sizeof(struct Matrix));
    if (newMatrixPointer == NULL) {
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    newMatrixPointer->height = (uint32_t*)malloc(sizeof(int));
    if (newMatrixPointer->height == NULL) {
        free(newMatrixPointer);
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    *(newMatrixPointer->height) = height;
    newMatrixPointer->width = (uint32_t*)malloc(sizeof(int));
    if (newMatrixPointer->width == NULL) {
        free(newMatrixPointer->height);
        free(newMatrixPointer);
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    *(newMatrixPointer->width) = width;
    newMatrixPointer->array = (double**)malloc(height * sizeof(double*));
    if (newMatrixPointer->array == NULL) {
        free(newMatrixPointer->height);
        free(newMatrixPointer->width);
        free(newMatrixPointer);
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    for (uint32_t i = 0; i < height; i++) {
        newMatrixPointer->array[i] = (double*)malloc(width * sizeof(double));
        if (newMatrixPointer->array[i] == NULL) {
            free(newMatrixPointer->height);
            free(newMatrixPointer->width);
            for (uint32_t j = 0; j < i; j++) {
                free(newMatrixPointer->array[j]);
            }
            free(newMatrixPointer->array);
            free(newMatrixPointer);
            return ERROR_FAILED_MEMORY_ALLOCATION;
        }
    }
    *matrix = newMatrixPointer;
    return ERROR_SUCCESS;
}

ErrorCode matrix_copy(PMatrix* result, CPMatrix source){
    ErrorCode code;
    uint32_t height;
    uint32_t width;
    double value;
    code = matrix_getHeight(source, &height);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(source, &width);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_create(result, height, width);
    if (!error_isSuccess(code)) {
        return code;
    }
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            code = matrix_getValue(source, i, j, &value);
            if (!error_isSuccess(code)) {
                return code;
            }
            code = matrix_setValue(*result, i, j, value);
            if (!error_isSuccess(code)) {
                return code;
            }
        }
    }
    return ERROR_SUCCESS;
}

void matrix_destroy(PMatrix matrix) {
    if (matrix != NULL) {
        for (uint32_t i = 0; i < *(matrix->height); i++) {
            free(matrix->array[i]);
        }
        free(matrix->array);
        free(matrix->height);
        free(matrix->width);
        free(matrix);
    }
}

ErrorCode matrix_getHeight(CPMatrix matrix, uint32_t* result) {
    if (matrix == NULL) {
        return ERROR_MISSING_MATRIX;
    }
    if (matrix->height == NULL) {
        return ERROR_MISSING_MATRIX_HEIGHT;
    }
    if (result == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    *result = *(matrix->height);
    return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    if (matrix == NULL) {
        return ERROR_MISSING_MATRIX;
    }
    if (matrix->width == NULL) {
        return ERROR_MISSING_MATRIX_WIDTH;
    }
    if (result == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    *result = *(matrix->width);
    return ERROR_SUCCESS;
}

ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double value) {
    if (matrix == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    if (matrix->array == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    if (matrix->array[rowIndex] == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    matrix->array[rowIndex][colIndex] = value;
    return ERROR_SUCCESS;
}

ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double* value) {
    if (matrix == NULL) {
        return ERROR_MISSING_MATRIX;
    }
    if (matrix->array == NULL) {
        return ERROR_MISSING_MATRIX_VALUES;
    }
    if (matrix->array[rowIndex] == NULL) {
        return ERROR_MISSING_MATRIX_VALUES;
    }
    if (value == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    *value = matrix->array[rowIndex][colIndex];
    return ERROR_SUCCESS;
}

ErrorCode matrix_add(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    ErrorCode code;
    uint32_t lhsHeight;
    uint32_t lhsWidth;
    uint32_t rhsHeight;
    uint32_t rhsWidth;
    double lhsValue;
    double rhsValue;
    code = matrix_getHeight(lhs, &lhsHeight);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(lhs, &lhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getHeight(rhs, &rhsHeight);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(rhs, &rhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    if (lhsHeight != rhsHeight) {
        return ERROR_INVALID_MATRIX_SIZES;
    }
    if (lhsWidth != rhsWidth) {
        return ERROR_INVALID_MATRIX_SIZES;
    }
    code = matrix_create(result, lhsHeight, lhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    for (uint32_t i = 0; i < lhsHeight; i++) {
        for (uint32_t j = 0; j < lhsWidth; j++) {
            code = matrix_getValue(lhs, i, j, &lhsValue);
            if (!error_isSuccess(code)) {
                matrix_destroy(*result);
                return code;
            }
            code = matrix_getValue(rhs, i, j, &rhsValue);
            if (!error_isSuccess(code)) {
                matrix_destroy(*result);
                return code;
            }
            code = matrix_setValue(*result, i, j, lhsValue + rhsValue);
            if (!error_isSuccess(code)) {
                matrix_destroy(*result);
                return code;
            }  
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    ErrorCode code;
    uint32_t lhsHeight;
    uint32_t lhsWidth;
    uint32_t rhsHeight;
    uint32_t rhsWidth;
    double lhsValue;
    double rhsValue;
    double resultValue;
    code = matrix_getHeight(lhs, &lhsHeight);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(lhs, &lhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getHeight(rhs, &rhsHeight);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(rhs, &rhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    if (lhsWidth != rhsHeight) {
        return ERROR_INVALID_MATRIX_SIZES;
    }
    code = matrix_create(result, lhsHeight, rhsWidth);
    if (!error_isSuccess(code)) {
        return code;
    }
    for (uint32_t i = 0; i < lhsHeight; i++) {
        for (uint32_t j = 0; j < rhsWidth; j++) {
            resultValue = 0;
            for (uint32_t k = 0; k < lhsWidth; k++) {
                code = matrix_getValue(lhs, i, k, &lhsValue);
                if (!error_isSuccess(code)) {
                    matrix_destroy(*result);
                    return code;
                }
                code = matrix_getValue(rhs, k, j, &rhsValue);
                if (!error_isSuccess(code)) {
                    matrix_destroy(*result);
                    return code;
                }
                resultValue += lhsValue * rhsValue;
            }
            code = matrix_setValue(*result, i, j, resultValue);
            if (!error_isSuccess(code)) {
                matrix_destroy(*result);
                return code;
            } 
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    ErrorCode code;
    uint32_t height;
    uint32_t width;
    double value;
    code = matrix_getHeight(matrix, &height);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(matrix, &width);
    if (!error_isSuccess(code)) {
        return code;
    }
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            code = matrix_getValue(matrix, i, j, &value);
            if (!error_isSuccess(code)) {
                return code;
            }
            code = matrix_setValue(matrix, i, j, scalar * value);
            if (!error_isSuccess(code)) {
                return code;
            }
        }
    }
    return ERROR_SUCCESS;
}
