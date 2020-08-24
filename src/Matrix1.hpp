#include "Matrix.h"

#ifndef MATRIX_H
#define MATRIX_H
class Matrix{

public:
    Matrix(int width, int height);
    int getHeight() const;
    int getWidth() const;
    void setValue(int i, int j, double value);
    double getValue(int i, int j) const;
    ~Matrix();
private:
    PMatrix matrix = nullptr;
    PMatrix getMatrix() const;
};
#endif