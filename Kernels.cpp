#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>

#include "Eigen/src/Core/Matrix.h"
#include "amx_helper.h"
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

    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            for (k = 0; k < M; k++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

std::string NaiveCacheAwareVectorizedKernel::getName() {
    return "Naive Cache Aware Vectorized Kernel";
}

void AMXTransposeKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    AMX_SET();
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    unsigned short pointwiseProduct[32] = {0};
    B.Transpose();

    size_t i, j, k, ending;
    for (i = 0; i < N; i++) {
        for (j = 0; j < P; j++) {
            for (k = 0; k < M; k += 32) {
                ending = std::min(k + 32, M);
                AMX_LDX(AMX_PTR(&A(i, k)) | LDX_REG(0));
                AMX_LDY(AMX_PTR(&B(j, k)) | LDY_REG(0)); // indicies are switched since we transpose B.
                AMX_MAC16(MAC16_VECTOR_MODE | MAC16_SKIP_Z | MAC16_X_OFFSET(0) | MAC16_Y_OFFSET(0) | MAC16_Z_ROW(0));
                AMX_STZ(AMX_PTR(&pointwiseProduct) | STZ_Z_ROW(0));
                for (int idx = k; idx < ending; idx++) {
                    C(i, j) += pointwiseProduct[idx - k];
                }
            }
        }
    }

    AMX_CLR();
}

std::string AMXTransposeKernel::getName() {
    return "AMX Transpose Kernel";
}

void AMXTransposeTiledKernel::multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) {
    AMX_SET();
    size_t N = A.numRows();
    size_t M = A.numCols();
    size_t P = B.numCols();

    if (A.numCols() != B.numRows())
        throw std::invalid_argument("Input dimension mismatch.");

    unsigned short pointwiseProduct[32] = {0};
    B.Transpose();

    size_t i, j, k, ii, jj, kk, ending;
    size_t T = static_cast<int>(std::sqrt(M));
    std::cout << "tile size: " << T << std::endl;
    int iters = 0;
    for (i = 0; i < N; i += T) {
        size_t iBoundary = std::min(i + T, N);
        for (j = 0; j < P; j += T) {
            size_t jBoundary = std::min(j + T, P);
            for (k = 0; k < M; k += T) {
                size_t kBoundary = std::min(k + T, M);
                for (ii = i; ii < iBoundary; ++ii) {
                    for (jj = j; jj < jBoundary; ++jj) {
                        for (kk = k; kk < kBoundary; ++kk) {
                            ending = std::min(kk + 32, kBoundary);
                            AMX_LDX(AMX_PTR(&A(ii, kk)) | LDX_REG(0));
                            AMX_LDY(AMX_PTR(&B(jj, kk)) | LDY_REG(0)); // indicies are switched since we transpose B.
                            AMX_MAC16(MAC16_VECTOR_MODE | MAC16_SKIP_Z | MAC16_X_OFFSET(0) | MAC16_Y_OFFSET(0) | MAC16_Z_ROW(0));
                            AMX_STZ(AMX_PTR(&pointwiseProduct) | STZ_Z_ROW(0));
                            for (int idx = kk; idx < ending; idx++) {
                                C(ii, jj) += pointwiseProduct[idx - kk];
                                iters += 1;
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "total iters: " << iters << std::endl;
    AMX_CLR();
}

std::string AMXTransposeTiledKernel::getName() {
    return "AMX Transpose Tiled Kernel";
}

void eigenMultiply(Matrix3i& A, Matrix3i& B) {
    Matrix3i C;
    C.noalias() += A * B;
    return;
}
