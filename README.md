# MatMul with Apple M1 AMX instruction set.

Important Files:

- `MMBenchmark.cpp` - benchmarking script.
- `MMTest.cpp` - simple test script to verify correctness of kernels.
- `MMKernel.h` - definition of matrix kernel base class.
- `Matrix.h` - definition of "matrix", which was the container used throughout this project.
- `Kernels.cpp / .h` - implementation of various kernels that were benchmarked.
- `Stats.h` - a collection of tools to record data.
- `amx_helper.h` – a header file that defines extra macros that makes the AMX instructions easier to use.

Dependencies:

- https://github.com/corsix/amx, make sure to clone this folder in the root directory.
