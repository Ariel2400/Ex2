#include "Matrix.h"

#ifndef MATRIX_H
#define MATRIX_H
class Matrix{

public:
    Matrix(int width, int height);
    int getHeight();
    int getWidth();
    void setValue(int i, int j, double value);
    double getValue(int i, int j);
    ~Matrix();
private:
    PMatrix matrix = nullptr;
};
#endif