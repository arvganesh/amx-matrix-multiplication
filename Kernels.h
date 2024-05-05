#ifndef _KERNEL_H
#define _KERNEL_H

#include "MMKernel.h"
#include <stdexcept>
#include <Eigen/Dense>

using Eigen::MatrixXi;

class NaiveKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

class TiledKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

class NaiveCacheAwareKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

class NaiveCacheAwareVectorizedKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

class AMXTransposeKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

class AMXTransposeTiledKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

void eigenMultiply(MatrixXi& A, MatrixXi& B, MatrixXi& C);

#endif
