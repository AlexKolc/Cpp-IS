#include <iostream>
#include "matrix.h"

using namespace std;

void Matrix::resize(int newSizeN, int newSizeM) {
    this->~Matrix();
    new(this) Matrix(newSizeN, newSizeM);
}

Matrix::Matrix() {
    this->sizeN = 0;
    this->sizeM = 0;
}

Matrix::Matrix(int n) {
    this->sizeN = n;
    this->sizeM = n;
    this->matrix = new int *[this->sizeN];
    for (int i = 0; i < this->sizeN; i++)
        this->matrix[i] = new int[this->sizeM];
}

Matrix::Matrix(int n, int m) {
    this->sizeN = n;
    this->sizeM = m;
    this->matrix = new int *[this->sizeN];
    for (int i = 0; i < this->sizeN; i++)
        this->matrix[i] = new int[this->sizeM];
}

Matrix::Matrix(const Matrix &that) {
    this->sizeN = that.sizeN;
    this->sizeM = that.sizeM;
    this->matrix = new int *[this->sizeN];
    for (int i = 0; i < this->sizeN; i++) {
        this->matrix[i] = new int[this->sizeM];
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] = that.matrix[i][j];
    }
}

Matrix::~Matrix() {
    if (this->matrix) {
        for (int i = 0; i < this->sizeN; i++)
            delete[] this->matrix[i];
        delete[] this->matrix;
    }
}

void Matrix::transpose() {
    Matrix oldMatrix = *this;
    this->resize(this->sizeM, this->sizeM);

    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] = oldMatrix.matrix[j][i];
}

void Matrix::dot(const Matrix &matrix) {
    if (this->sizeM != matrix.sizeN)
        throw runtime_error("The sizes of the matrices don't match");

    Matrix newMatrix(*this);
    this->resize(this->sizeN, matrix.sizeM);
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++) {
            this->matrix[i][j] = 0;
            for (int k = 0; k < matrix.sizeN; k++)
                this->matrix[i][j] += newMatrix.matrix[i][k] * matrix.matrix[k][j];
        }
}

int Matrix::operator()(int i, int j) const {
    return this->matrix[i][j];
}

int *Matrix::operator[](int i) const {
    return this->matrix[i];
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    this->resize(matrix.sizeN, matrix.sizeM);
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] = matrix.matrix[i][j];
    return *this;
}

Matrix Matrix::operator+(const Matrix &matrix) const {
    Matrix resultMatrix(*this);
    resultMatrix += matrix;
    return resultMatrix;
}

Matrix Matrix::operator+(const int &lambda) const {
    Matrix resultMatrix(*this);
    resultMatrix += lambda;
    return resultMatrix;
}

Matrix Matrix::operator-(const Matrix &matrix) const {
    Matrix resultMatrix(*this);
    resultMatrix -= matrix;
    return resultMatrix;
}

Matrix Matrix::operator-(const int &lambda) const {
    Matrix resultMatrix(*this);
    resultMatrix -= lambda;
    return resultMatrix;
}

Matrix Matrix::operator*(const Matrix &matrix) const {
    Matrix resultMatrix(*this);
    resultMatrix *= matrix;
    return resultMatrix;
}

Matrix Matrix::operator*(const int &lambda) const {
    Matrix resultMatrix(*this);
    resultMatrix *= lambda;
    return resultMatrix;
}

Matrix &Matrix::operator+=(const Matrix &matrix) {
    if (this->sizeN != matrix.sizeN || this->sizeM != matrix.sizeM)
        throw runtime_error("The sizes of the matrices don't match");

    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] += matrix.matrix[i][j];
    return *this;
}

Matrix &Matrix::operator+=(const int &lambda) {
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] += lambda;
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &matrix) {
    if (this->sizeN != matrix.sizeN || this->sizeM != matrix.sizeM)
        throw runtime_error("The sizes of the matrices don't match");

    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] -= matrix.matrix[i][j];
    return *this;
}

Matrix &Matrix::operator-=(const int &lambda) {
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] -= lambda;
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &matrix) {
    if (this->sizeM != matrix.sizeN)
        throw runtime_error("The sizes of the matrices don't match");

    Matrix newMatrix(*this);
    this->resize(this->sizeN, matrix.sizeM);
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++) {
            this->matrix[i][j] = 0;
            for (int k = 0; k < matrix.sizeN; k++)
                this->matrix[i][j] += newMatrix.matrix[i][k] * matrix.matrix[k][j];
        }
    return *this;
}

Matrix &Matrix::operator*=(const int &lambda) {
    for (int i = 0; i < this->sizeN; i++)
        for (int j = 0; j < this->sizeM; j++)
            this->matrix[i][j] *= lambda;
    return *this;
}

istream &operator>>(istream &stream, Matrix &matrix) {
    for (int i = 0; i < matrix.sizeN; i++)
        for (int j = 0; j < matrix.sizeM; j++)
            stream >> matrix.matrix[i][j];
    return stream;
}

ostream &operator<<(ostream &stream, const Matrix &matrix) {
    for (int i = 0; i < matrix.sizeN; i++) {
        for (int j = 0; j < matrix.sizeM; j++)
            stream << matrix.matrix[i][j] << "\t";
        stream << endl;
    }
    return stream;
}
