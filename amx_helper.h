#include "amx/aarch64.h"

#define AMX_PTR(ptr) ((uint64_t)(ptr))
#define LDX_REG(reg) ((uint64_t)(reg) << 56)
#define LDY_REG(reg) ((uint64_t)(reg) << 56)
#define LDZ_REG(reg) ((uint64_t)(reg) << 56)

#define MAC16_VECTOR_MODE 1ULL << 63
#define MAC16_MATRIX_MODE 0ULL << 63
#define MAC16_X_OFFSET(bytes) ((uint64_t)(bytes) << 10)
#define MAC16_X_REG(reg) (MAC16_X_OFFSET((reg)*64))
#define MAC16_Y_OFFSET(bytes) ((uint64_t)(bytes) << 0)
#define MAC16_Y_REG(reg) (MAC16_Y_OFFSET((reg)*64))
#define MAC16_Z_ROW(row) ((uint64_t)(row) << 20)
#define MAC16_SKIP_Z 1ULL << 27

#define STZ_Z_ROW(row) ((uint64_t)(row) << 56)
#define STZ_STORE_SINGLE 0ULL < 62
