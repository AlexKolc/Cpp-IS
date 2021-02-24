#include "matrix.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream fin("C:\\Users\\alexx\\Desktop\\studies\\2semestr\\C++\\lab1\\in.txt");
    freopen("C:\\Users\\alexx\\Desktop\\studies\\2semestr\\C++\\lab1\\out.txt", "w", stdout);
    int n, m;

    fin >> n >> m;
    Matrix matrix1(n, m);
    fin >> matrix1;
    cout << "matrix1 = \n" << matrix1 << endl;

    fin >> n >> m;
    Matrix matrix2(n, m);
    fin >> matrix2;
    cout << "matrix2 = \n" << matrix2 << endl;

    cout << "matrix1 + matrix2 = \n" << matrix1 + matrix2 << endl;

    cout << "matrix1 - matrix2 = \n" << matrix1 - matrix2 << endl;

    cout << "matrix1 * matrix2 = \n" << matrix1 * matrix2 << endl;

    cout << "matrix1 + 4 = \n" << matrix1 + 4 << endl;

    cout << "matrix1 - 4 = \n" << matrix1 - 4 << endl;

    cout << "matrix1 * 4 = \n" << matrix1 * 4 << endl;

    cout << "matrix1(1, 1) = " << matrix1(1, 1) << endl << endl;

    cout << "matrix1[1][1] = " << matrix1[1][1] << endl << endl;

    Matrix matrix3 = matrix1;
    cout << "matrix3 = matrix1 = \n" << matrix3 << endl;

    matrix3 += 1;
    cout << "matrix3 += 1 = \n" << matrix3 << endl;

    matrix3 -= 4;
    cout << "matrix3 -= 4 = \n" << matrix3 << endl;

    matrix3 *= 2;
    cout << "matrix3 *= 2 = \n" << matrix3 << endl;

    matrix3 += matrix1;
    cout << "matrix3 += matrix1 = \n" << matrix3 << endl;

    matrix3 -= matrix2;
    cout << "matrix3 -= matrix2 = \n" << matrix3 << endl;

    matrix3 *= matrix1;
    cout << "matrix3 *= matrix1 = \n" << matrix3 << endl;

    matrix1.dot(matrix2);
    cout << "matrix1.dot(matrix2) = \n" << matrix1 << endl;

    matrix1.transpose();
    cout << "matrix1.transpose() = \n" << matrix1 << endl;

    fin.close();
    return 0;
}
