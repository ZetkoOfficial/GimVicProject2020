/*
    Linear algebra library with support for vector addition and matrix-vector multiplication.
    
    Author: Anže Hočevar
*/

#ifndef INCLUDE_VECTORMATRIX
#define INCLUDE_VECTORMATRIX

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Vector2{
    vector<double> vals;

    Vector2(double a, double b){
        this->vals = {a, b};
    }

    const double& operator[] (int index) const {
        return vals[index];
    }

    double& operator[] (int index) {
        return vals[index];
    }

    friend Vector2 operator+(const Vector2& vec0, const Vector2& vec1) {
        Vector2 newVec (vec0[0] + vec1[0], vec0[1] + vec1[1]);

        return newVec;
    }

    friend Vector2 operator*(double d, const Vector2& vec) {
        Vector2 newVec (vec[0] * d, vec[1] * d);

        return newVec;
    }

    friend ostream& operator<<(ostream& os, const Vector2& vec){
        os << "|" << vec.vals[0] << "|" << "\n";
        os << "|" << vec.vals[1] << "|";

        return os;
    }
};

struct Matrix2x2 {
    vector<double> row0, row1;
    
    Matrix2x2(const vector<double>& row0, const vector<double>& row1) {
        this->row0 = row0;
        this->row1 = row1;
    }

    friend Matrix2x2 operator*(double d, const Matrix2x2& matrix) {
        Matrix2x2 newMat ({matrix.row0[0] * d, matrix.row0[1] * d},
                          {matrix.row1[0] * d, matrix.row1[1] * d});

        return newMat;
    }

    friend Vector2 operator*(const Matrix2x2& matrix, const Vector2& vec) {
        Vector2 newVec (matrix.row0[0] * vec[0] + matrix.row0[1] * vec[1], matrix.row1[0] * vec[0] + matrix.row1[1] * vec[1]);

        return newVec;
    }

    friend ostream& operator<<(ostream& os, const Matrix2x2& mat) {
        os << "|" << mat.row0[0] << " " << mat.row0[1] << "|" << "\n";
        os << "|" << mat.row1[0] << " " << mat.row1[1] << "|";

        return os;
    }
};

#endif