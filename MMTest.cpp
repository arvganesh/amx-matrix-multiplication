#include "Kernels.h"
#include "MMKernel.h"
#include "Matrix.h"
#include <arm/_types.h>
#include <array>
#include <ios>

bool matrixCompare(IntMatrix& A, IntMatrix&B) {
    int aN = A.numRows();
    int aM = A.numCols();
    int bN = B.numRows();
    int bM = B.numCols();

    if (aN != bN || aM != bM)
        return false;

    for (int i = 0; i < aN; ++i) {
        for (int j = 0; j < aM; ++j) {
            if (A(i, j) != B(i, j))
                 return false;
        }
    }

    return true;
}

void runTests() {
    IntMatrix A = {
        {2, 24, 3},
        {39, 21, 1},
        {85, 9, 25}
    };
    IntMatrix B = {
        {23, 99, 83},
        {72, 40, 23},
        {19, 7, 3}
    };
    IntMatrix C_Expected = {
        {1831, 1179, 727},
        {2428, 4708, 3723},
        {3078, 8950, 7337}
    };

    std::vector<std::unique_ptr<MMKernel> > kernels;
    kernels.push_back(std::make_unique<TiledKernel>());
    kernels.push_back(std::make_unique<NaiveKernel>());
    kernels.push_back(std::make_unique<NaiveCacheAwareKernel>());

    for (auto& kernel : kernels) {
        IntMatrix C_Actual(3);
        std::cout << "Testing: " << kernel->getName() <<  " – ";
        kernel->multiply(A, B, C_Actual);
        if (!matrixCompare(C_Actual, C_Expected)) {
            std::cout << "Failed." << std::endl;
            std::cout << C_Actual << std::endl;
        } else {
            std::cout << "Success!" << std::endl;
        }
    }
}

void compare() {
    int N = 2;
    IntMatrix A(N, true);
    IntMatrix B(N, true);
    IntMatrix C_Naive(N);
    IntMatrix C_Test(N);

    NaiveKernel k1;
    AMXKernel k2;

    k1.multiply(A, B, C_Naive);
    k2.multiply(A, B, C_Test);

    if (!matrixCompare(C_Naive, C_Test)) {
        std::cout << "Test Failed" << std::endl;
        std::cout << "Expected ";
        std::cout << C_Naive << std::endl;
        std::cout << "Actual ";
        std::cout << C_Test << std::endl;
    }
}

int main() {
    // runTests();
    compare();
    return 0;
}
