#include <stddef.h>
#include <stdlib.h>

#include "Matrix.h"
#include "ErrorCode.h"

struct Matrix {
    int* height;
    int* width;
    double** array;
};

/**
 * @brief Creates a new matrix of a given height an width,
 *  all values are initially zeroes.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the created matrix.
 * @param[in] height Height of the matrix
 * @param[in] width Width of the matrix
 * @return ErrorCode
 */
ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    if (matrix == NULL) {
        return ERROR_NULL_OUTPUT_POINTER;
    }
    PMatrix newMatrixPointer = (PMatrix)malloc(sizeof(struct Matrix));
    if (newMatrixPointer == NULL) {
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    newMatrixPointer->height = (int*)malloc(sizeof(int));
    if (newMatrixPointer->height == NULL) {
        free(newMatrixPointer);
        return ERROR_FAILED_MEMORY_ALLOCATION;
    }
    *(newMatrixPointer->height) = height;
    newMatrixPointer->width = (int*)malloc(sizeof(int));
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
    for (int i = 0; i < height; i++) {
        newMatrixPointer->array[i] = (double*)malloc(width * sizeof(double));
        if (newMatrixPointer->array[i] == NULL) {
            free(newMatrixPointer->height);
            free(newMatrixPointer->width);
            for (int j = 0; j < i; j++) {
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

/**
 * @brief Creates a new matrix from an old matrix.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the copied matrix.
 * @param[in] source The matrix to copy.
 * @return ErrorCode
 */
ErrorCode matrix_copy(PMatrix* result, CPMatrix source){
    ErrorCode code;
    int height;
    int width;
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
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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

/**
 * @brief Destroys a matrix.
 *
 * @param matrix the matrix to destroy.
 */
void matrix_destroy(PMatrix matrix) {
    if (matrix != NULL) {
        for (int i = 0; i < *(matrix->height); i++) {
            free(matrix->array[i]);
        }
        free(matrix->array);
        free(matrix->height);
        free(matrix->width);
        free(matrix);
    }
}

/**
 * @brief Returns the height of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
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

/**
 * @brief Returns the width of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
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

/**
 * @brief Sets a value to the matrix.
 *
 * @param[in, out] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to set.
 * @param[in] colIndex Column index of the value to setF.
 * @param[in] value Value to set.
 * @return ErrorCode
 */
ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double value) {
    ErrorCode code;
    int height;
    int width;
    code = matrix_getHeight(matrix, &height);
    if (!error_isSuccess(code)) {
        return code;
    }
    if (rowIndex >= height) {
        return ERROR_INVALID_MATRIX_SIZES;
    }
    code = matrix_getWidth(matrix, &width);
    if (!error_isSuccess(code)) {
        return code;
    }
    if (colIndex >= width) {
        return ERROR_INVALID_MATRIX_SIZES;
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

/**
 * @brief Sets a value to the matrix.
 *
 * @param[in] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to get.
 * @param[in] colIndex Column index of the value to get.
 * @param[out] value The address of a double variable to receive
 *  the value from the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double* value) {
    ErrorCode code;
    int height;
    int width;
    code = matrix_getHeight(matrix, &height);
    if (!error_isSuccess(code)) {
        return code;
    }
    if (rowIndex >= height) {
        return ERROR_INVALID_MATRIX_SIZES;
    }
    code = matrix_getWidth(matrix, &width);
    if (!error_isSuccess(code)) {
        return code;
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

/**
 * @brief Computes the addition of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the addition operation.
 * @param[in] rhs The right hand side of the addition operation.
 * @return ErrorCode
 */
ErrorCode matrix_add(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    ErrorCode code;
    int lhsHeight;
    int lhsWidth;
    int rhsHeight;
    int rhsWidth;
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
    for (int i = 0; i < lhsHeight; i++) {
        for (int j = 0; j < lhsWidth; j++) {
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

/**
 * @brief Computes the multiplication of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the multiplication operation.
 * @param[in] rhs The right hand side of the multiplication operation.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    ErrorCode code;
    int lhsHeight;
    int lhsWidth;
    int rhsHeight;
    int rhsWidth;
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
    for (int i = 0; i < lhsHeight; i++) {
        for (int j = 0; j < rhsWidth; j++) {
            resultValue = 0;
            for (int k = 0; k < lhsWidth; k++) {
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

/**
 * @brief Multiplies a matrix with a scalar and stores the result in
 *  the given matrix.
 *
 * @param[in, out] matrix On input, the matrix to multiply with a scalar.
 *  On output, the result of the multiplication operation.
 * @param[in] scalar The scalar to multiply with.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    ErrorCode code;
    int height;
    int width;
    double value;
    code = matrix_getHeight(matrix, &height);
    if (!error_isSuccess(code)) {
        return code;
    }
    code = matrix_getWidth(matrix, &width);
    if (!error_isSuccess(code)) {
        return code;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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
