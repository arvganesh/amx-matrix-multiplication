#ifndef _MM_KERNEL_H
#define _MM_KERNEL_H

#include <vector>
#include "Matrix.h"

class MMKernel {
    public:
        virtual ~MMKernel() = default;
        virtual void multiply(IntMatrix& A, IntMatrix& B, IntMatrix&C) = 0;
        virtual std::string getName() = 0;
};

#endif
