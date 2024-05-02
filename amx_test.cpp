#include "amx/aarch64.h"
#include <_types/_uint16_t.h>
#include <arm/_types.h>
#include <cstring>
#include <stdbool.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include "Matrix.h"

#define AMX_PTR(ptr) ((uint64_t)(ptr))
#define LDX_REG(reg) ((uint64_t)(reg) << 56)
#define LDY_REG(reg) ((uint64_t)(reg) << 56)
#define LDZ_REG(reg) ((uint64_t)(reg) << 56)

#define VECTOR_MODE 1ULL << 63
#define MATRIX_MODE 0ULL << 63
#define MAC16_X_OFFSET(bytes) ((uint64_t)(bytes) << 10)
#define MAC16_X_REG(reg) (MAC16_X_OFFSET((reg)*64))
#define MAC16_Y_OFFSET(bytes) ((uint64_t)(bytes) << 0)
#define MAC16_Y_REG(reg) (MAC16_Y_OFFSET((reg)*64))
#define MAC16_Z_ROW(row) ((uint64_t)(row) << 20)


#define SKIP_Z 1ULL << 27
#define STZ_Z_ROW(row) ((uint64_t)(row) << 56)
#define STORE_SINGLE 0ULL < 62

void print_bits(unsigned short value) {
    int i;
    // Start from the most significant bit (bit 15) to the least significant bit (bit 0)
    for (i = 15; i >= 0; i--) {
        // Check the value of the current bit using bitwise AND with a bitmask
        if (value & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }

        // Add a space for readability, e.g., print a space after every 4 bits
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("- %hu", value);
    printf("\n");
}

int main() {
    AMX_SET();
    __uint16_t f[4] = {1, 2, 3, 4};
    __uint16_t g[4] = {5, 6, 7, 8};

    __uint16_t xy[32];
    char zeroes[64] = {0};
    // Matrix<__uint16_t> A = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    // Matrix<__uint16_t> B = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    std::array<std::array<__uint16_t, 3>, 3> A = {{{5, 8, 2}, {8, 3, 1}, {5, 3, 9}}};
    std::array<std::array<__uint16_t, 3>, 3> B = {{{5, 8, 2}, {8, 3, 1}, {5, 3, 9}}};

    Matrix<__uint16_t> C(4);

    // __uint16_t* aPtr = &A(0, 0);
    // __uint16_t* bPtr = &B(0, 0);
    __uint16_t* aPtr = &A[0][0];
    __uint16_t* bPtr = &B[0][0];

    // std::cout << A << std::endl;
    // std::cout << B << std::endl;

    AMX_LDX(AMX_PTR(aPtr) | LDX_REG(0));
    AMX_LDY(AMX_PTR(bPtr) | LDY_REG(0));
    AMX_LDZ(AMX_PTR(zeroes) | LDZ_REG(0));
    AMX_MAC16(VECTOR_MODE | SKIP_Z | MAC16_Z_ROW(0));

    for (int r = 0; r < 32; r++) {
        AMX_STZ(AMX_PTR(&xy) | STZ_Z_ROW(r));
        for (int i = 0; i < 32; i++) {
            printf("%hu, ", xy[i]);
            // print_bits(xy[i]);
        }
        printf("\n");
    }

    std::cout << C << std::endl;

    AMX_CLR();
    return 0;
}
