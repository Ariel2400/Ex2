#include "Matrix.h"
#pragma once
class Matrix{
    private:
        PMatrix matrix = nullptr;
    public:
    Matrix(int width, int height);
    int getHeight();
    int getWidth();
    void setValue(int i, int j, double value);
    double getValue(int i, int j);
    ~Matrix();
};