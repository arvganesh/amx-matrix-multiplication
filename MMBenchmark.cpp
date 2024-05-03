#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <Eigen/Dense>
#include <stdlib.h>
#include "Kernels.h"
#include "MMKernel.h"
#include "Stats.h"

using Eigen::Matrix3i;

int benchmarkKernel(std::unique_ptr<MMKernel> kernel, int startDim, int numIters, int increment, Stats& stats) {
    std::cout << "Benchmarking: " << kernel->getName() << std::endl;
    bool shouldAlign32 = kernel->getName() == "AMX Kernel";
    for (int N = startDim; N < startDim + numIters * increment; N += increment) {
        IntMatrix A(N, true, shouldAlign32); // random NxN
        IntMatrix B(N, true, shouldAlign32); // random NxN
        IntMatrix C(N); // zero-filled NxN
        B.Transpose(); // B is in "column-major" order.

        // Start recording stats.
        auto start = std::chrono::high_resolution_clock::now();
        kernel->multiply(A, B, C);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Dim " << N << " took " << elapsed_time.count() << std::endl;
        stats[kernel->getName()].add(elapsed_time.count());
    }
    return 0;
}

int benchmarkEigen(int startDim, int numIters, int increment, Stats& stats) {
    std::cout << "Benchmarking: Eigen" << std::endl;
    for (int N = startDim; N < startDim + numIters * increment; N += increment) {
        Matrix3i A = Matrix3i::Random();
        Matrix3i B = Matrix3i::Random();

        // Start recording stats.
        auto start = std::chrono::high_resolution_clock::now();
        eigenMultiply(A, B);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Dim " << N << " took " << elapsed_time.count() << std::endl;
        stats["Eigen"].add(elapsed_time.count());
    }
    return 0;
}

void run(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<std::unique_ptr<MMKernel> > kernels_to_test;
    int seed = 0;
    int startDim = 1;
    int numIters = 1000;
    int increment = 1;
    bool eigen = false;
    for (auto it = args.begin(); it != args.end(); ++it) {
        if (*it == "naive") {
            kernels_to_test.push_back(std::make_unique<NaiveKernel>());
        }
        if (*it == "naive-cache") {
            kernels_to_test.push_back(std::make_unique<NaiveCacheAwareKernel>());
        }
        if (*it == "naive-cache-vec") {
            kernels_to_test.push_back(std::make_unique<NaiveCacheAwareVectorizedKernel>());
        }
        if (*it == "tiled") {
            kernels_to_test.push_back(std::make_unique<TiledKernel>());
        }
        if (*it == "amx") {
            kernels_to_test.push_back(std::make_unique<AMXKernel>());
        }
        if (*it == "seed") {
            if (*(++it) == "random")
                seed = std::time(0);
            std::srand(seed);
            std::cout << "Random Seed: " << seed << std::endl;
        }
        if (*it == "dim") {
            startDim = std::stoi(*(++it));
        }
        if (*it == "iters") {
            numIters = std::stoi(*(++it));
        }
        if (*it == "inc") {
            increment = std::stoi(*(++it));
        }
        if (*it == "eigen") {
            eigen = true;
        }
    }

    Stats stats;
    for (auto& kernel : kernels_to_test) {
        benchmarkKernel(std::move(kernel), startDim, numIters, increment, stats);
    }

    if (eigen) {
        benchmarkEigen(startDim, numIters, increment, stats);
    }

    for (auto& element : stats) {
        std::cout << element.first << " " << element.second << std::endl;
    }

    dumpStatsToCSV(stats);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        throw std::invalid_argument("Provide a kernel to benchmark.");
    }
    run(argc, argv);
    return 0;
}
