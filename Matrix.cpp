
#include <iostream>
using namespace std;

#include "Matrix.h"

class Matrix{
  public:
    PMatrix matrix = nullptr;
    Matrix(int height, int width){
      matrix_create(&matrix, height, width);
    }
    int getHeight(){
      uint32_t height;
      matrix_getHeight(matrix, &height);
      return height;
    }
    int getWidth(){
      uint32_t width;
      matrix_getWidth(matrix, &width);
      return width;
    }
    void setValue(int i, int j ,double value){
        matrix_setValue(matrix, i, j, value);
    }
    double getValue(int i, int j){
      double result;
      matrix_getValue(matrix, i, j, &result);
      return result;
    }
    
    ~Matrix(){
      matrix_destroy(matrix);
    }
};

int main() {
  Matrix matrix(4, 4);
  for (int i = 0; i < matrix.getHeight(); i++) {
    for (int j = 0; j < matrix.getWidth(); j++) {
      matrix.setValue(i, j, i * 10 + j);
    }
  }
  for (int i = 0; i < matrix.getHeight(); i++) {
    for (int j = 0; j < matrix.getWidth(); j++) {
      cout << matrix.getValue(i, j) << " ";
    }
    cout << endl;
  }
}