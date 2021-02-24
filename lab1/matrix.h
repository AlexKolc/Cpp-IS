#ifndef LAB1_MATRIX_H
#define LAB1_MATRIX_H

#include <iostream>

using namespace std;

class Matrix {
private:
    int **matrix;
    int sizeN;
    int sizeM;

    void resize(int, int);

public:
    Matrix();

    Matrix(int);

    Matrix(int, int);

    Matrix(const Matrix &);

    ~Matrix();

    void transpose();

    void dot(const Matrix &);

    int operator()(int, int) const;

    int *operator[](int) const;

    Matrix &operator=(const Matrix &);

    Matrix operator+(const Matrix &) const;

    Matrix operator+(const int &) const;

    Matrix operator-(const Matrix &) const;

    Matrix operator-(const int &) const;

    Matrix operator*(const Matrix &) const;

    Matrix operator*(const int &) const;

    Matrix &operator+=(const Matrix &);

    Matrix &operator+=(const int &);

    Matrix &operator-=(const Matrix &);

    Matrix &operator-=(const int &);

    Matrix &operator*=(const Matrix &);

    Matrix &operator*=(const int &);

    friend istream &operator>>(istream &, Matrix &);

    friend ostream &operator<<(ostream &, const Matrix &);
};

#endif //LAB1_MATRIX_H
