
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

