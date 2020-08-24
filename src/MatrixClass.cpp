
#include <iostream>
using namespace std;
#include "Matrix1.hpp"

Matrix::Matrix(int height, int width) { matrix_create(&matrix, height, width); }
PMatrix Matrix::getMatrix() const{
  return this->matrix;
}
int Matrix::getHeight() const {
  uint32_t height;
  matrix_getHeight(matrix, &height);
  return height;
}
int Matrix::getWidth() const {
  uint32_t width;
  matrix_getWidth(matrix, &width);
  return width;
}
void Matrix::setValue(int i, int j, double value) {
  matrix_setValue(matrix, i, j, value);
}
double Matrix::getValue(int i, int j) const {
  double result;
  matrix_getValue(matrix, i, j, &result);
  return result;
}

Matrix::~Matrix() { matrix_destroy(matrix); }
