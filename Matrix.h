#ifndef _MATRIX_H
#define _MATRIX_H

#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <stdlib.h>
#include <string>
#include <iomanip>

template<typename T>
class Matrix {
    public:
        Matrix(int n, int m, bool shouldRandomFill = false, bool shouldAlign32 = false) : rows(n), cols(m) {
            size_t size = (n * m);
            if (shouldAlign32 && size % 32 != 0) {
                size += (32 - size % 32);
            }
            data.resize(size);
            if (shouldRandomFill) {
                randomFillMatrix();
            } else {
                zeroFillMatrix();
            }
        }

        Matrix(int n, bool shouldRandomFill = false, bool shouldAlign32 = false) : Matrix(n, n, shouldRandomFill, shouldAlign32) {}

        Matrix(std::initializer_list<std::vector<int> > l) {
            for (auto& vec : l) {
                data.insert(data.end(), vec.begin(), vec.end());
            }
        }

        T& operator()(size_t row, size_t col) {
            return data.at(row * rows + col);
        }

        const T& operator()(size_t row, size_t col) const {
            return data.at(row * rows + col);
        }

        int numRows() {
            return rows;
        }

        int numCols() {
            return cols;
        }

        friend std::ostream& operator<<(std::ostream&, Matrix<T>& m) {
            // Print the matrix with aligned columns
            std::cout << m.numRows() << "x" << m.numCols() << " matrix: " << std::endl;
            for (size_t row = 0; row < m.numRows(); ++row) {
                for (size_t col = 0; col < m.numCols(); ++col) {
                    std::cout << std::setw(9) << std::right << m(row, col);
                    if (col != m.numCols() - 1) {
                        std::cout << " "; // Add space between columns
                    }
                }
                std::cout << "\n"; // Move to the next row
            }
            return std::cout;
        }

        void Transpose() {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < i; j++) {
                    int tmp = data[i * rows + j];
                    data[i * rows + j] = data[j * rows + i];
                    data[j * rows + i] = tmp;
                }
            }
        }

    private:
        std::vector<T> data;
        int rows;
        int cols;
        static T getRandomNumber() {
            return std::rand() % 50;
        }

        void randomFillMatrix() {
            std::generate(data.begin(), data.end(), getRandomNumber);
            return;
        }

        void zeroFillMatrix() {
            std::fill(data.begin(), data.end(), 0);
        }
};

typedef Matrix<unsigned short> IntMatrix;
#endif
