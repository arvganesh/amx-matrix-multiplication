#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>

#include "Eigen/src/Core/Matrix.h"
#include "sse2neon/sse2neon.h"
#include "Kernels.h"

using Eigen::Matrix3i;

/* NAIVE KERNEL */
void NaiveKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < P; ++j) {
            for (int k = 0; k < M; ++k) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

std::string NaiveKernel::getName() {
    return "Naive Kernel";
}

/* TILED KERNEL */
void TiledKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    size_t T = static_cast<int>(std::sqrt(M));
    for (int i = 0; i < N; i += T) {
        int iBoundary = std::min(i + T, N);
        for (int j = 0; j < P; j += T) {
            int jBoundary = std::min(j + T, P);
            for (int k = 0; k < M; k += T) {
                int kBoundary = std::min(k + T, M);
                for (int ii = i; ii < iBoundary; ++ii) {
                    for (int jj = j; jj < jBoundary; ++jj) {
                        for (int kk = k; kk < kBoundary; ++kk) {
                            C(ii, jj) += A(ii, kk) * B(kk, jj);
                        }
                    }
                }
            }
        }
    }
}

std::string TiledKernel::getName() {
    return "Tiled Kernel";
}

/* NAIVE CACHE AWARE KERNEL */
void NaiveCacheAwareKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < M; ++k) {
            for (int j = 0; j < P; ++j) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

std::string NaiveCacheAwareKernel::getName() {
    return "Naive Cache Aware Kernel";
}

/* NAIVE CACHE AWARE KERNEL */
void NaiveCacheAwareVectorizedKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    std::cout << A << std::endl;
    A.Transpose();
    std::cout << A << std::endl;
    B.Transpose();

    int i, j, k;
    __m128 a, b, c;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            for (k = 0; k < M; k++) {
                const __m128i* aPtr = (__m128i*) &A(i, k);
                const __m128i* bPtr = (__m128i*) &B(k, j);
                a = _mm_load_si128(aPtr);
                b = _mm_load_si128(bPtr);
                c = _mm_mul_epi32(a, b);
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

std::string NaiveCacheAwareVectorizedKernel::getName() {
    return "Naive Cache Aware Vectorized Kernel";
}

void eigenMultiply(Matrix3i& A, Matrix3i& B) {
    Matrix3i C;
    C.noalias() += A * B;
    return;
}
