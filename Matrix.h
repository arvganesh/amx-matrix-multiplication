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

template<typename T, unsigned R, unsigned C>
class Matrix {
    public:
        Matrix(int n, int m, bool shouldRandomFill = false) {
            if (shouldRandomFill) {
                randomFillMatrix();
            } else {
                zeroFillMatrix();
            }
        }

        Matrix(int n, bool shouldRandomFill = false) : Matrix(n, n, shouldRandomFill) {}

        Matrix(std::initializer_list<std::array<std::array<T, C>, R> > l) : data(l) {}

        T& operator()(size_t row, size_t col) {
            return data[row].at(col);
        }

        const T& operator()(size_t row, size_t col) const {
            return data[row].at(col);
        }

        int numRows() {
            return R;
        }

        int numCols() {
            return C;
        }

        friend std::ostream& operator<<(std::ostream&, Matrix<T, R, C>& m) {
            // Print the matrix with aligned columns
            std::cout << m.numRows() << "x" << m.numCols() << " matrix: " << std::endl;
            for (const auto& row : m.data) {
                for (size_t col = 0; col < row.size(); ++col) {
                    std::cout << std::setw(9) << std::right << row[col];
                    if (col != row.size() - 1) {
                        std::cout << " "; // Add space between columns
                    }
                }
                std::cout << "\n"; // Move to the next row
            }
            return std::cout;
        }

        void Transpose() {
            for (int i = 0; i < R; i++) {
                for (int j = 0; j < i; j++) {
                    int tmp = data[i][j];
                    data[i][j] = data[j][i];
                    data[j][i] = tmp;
                }
            }
        }

    private:
        std::array<std::array<T, C>, R> data;
        static T getRandomNumber() {
            return std::rand() % 50;
        }

        static void randomFillRow(std::array<T, C>& row) {
            std::generate(row.begin(), row.end(), getRandomNumber);
        }

        void randomFillMatrix() {
            std::for_each(data.begin(), data.end(), randomFillRow);
            return;
        }

        void zeroFillMatrix() {
            for (auto& row : data) {
                std::fill(row.begin(), row.end(), 0);
            }
        }
};

typedef Matrix<int, 3, 3> IntMatrix;
#endif
