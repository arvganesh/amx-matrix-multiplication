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
    // Matrix<__uint16_t> A = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 11, 60, 60}};
    // Matrix<__uint16_t> B = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 12, 24, 5}};
    // Matrix<__uint16_t> C = {{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};
    // Matrix<__uint16_t> D = {{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}};

    IntMatrix A = {{2}};
    IntMatrix B = {{6, 7, 8}};

    AMX_LDX(AMX_PTR(&A(0, 0)) | LDX_REG(0));
    AMX_LDY(AMX_PTR(&B(0, 0)) | LDY_REG(0));
    // AMX_LDX(AMX_PTR(&C(0, 0)) | LDX_REG(1));
    // AMX_LDY(AMX_PTR(&D(0, 0)) | LDY_REG(1));
    // AMX_LDX(AMX_PTR(&C(0, 0)) | LDX_REG(2));
    // AMX_LDY(AMX_PTR(&D(0, 0)) | LDY_REG(2));


    AMX_MAC16(MATRIX_MODE | SKIP_Z | MAC16_X_OFFSET(0) | MAC16_Y_OFFSET(0) | MAC16_Z_ROW(0));
    // AMX_MAC16(VECTOR_MODE | SKIP_Z | MAC16_X_OFFSET(64) | MAC16_Y_OFFSET(64) | MAC16_Z_ROW(1));
    // AMX_MAC16(VECTOR_MODE | SKIP_Z | MAC16_X_OFFSET(128) | MAC16_Y_OFFSET(128) | MAC16_Z_ROW(2));
    // AMX_MAC16(VECTOR_MODE | SKIP_Z | MAC16_X_OFFSET(0) | MAC16_Y_OFFSET(0) | MAC16_Z_ROW(0));

    for (int r = 0; r < 32; r++) {
        AMX_STZ(AMX_PTR(&xy) | STZ_Z_ROW(r));
        for (int i = 0; i < 32; i++) {
            printf("%u, ", xy[i]);
            // print_bits(xy[i]);
        }
        printf("\n");
    }

    // std::cout << C << std::endl;

    AMX_CLR();
    return 0;
}
