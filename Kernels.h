#ifndef _KERNEL_H
#define _KERNEL_H

#include "MMKernel.h"
#include <stdexcept>
#include <Eigen/Dense>

using Eigen::Matrix3i;

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

class AMXKernel : public MMKernel {
    public:
        void multiply(IntMatrix& A /* NxM */, IntMatrix& B /* MxP */, IntMatrix& C) override;
        std::string getName() override;
};

void eigenMultiply(Matrix3i& A, Matrix3i& B);

#endif
