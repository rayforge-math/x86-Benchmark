#include "benchmarks_impl.h"

typedef void (*BenchBody)(uint32_t loops);

typedef struct {
    const char* label;
    BenchBody body;
} BenchmarkItem;

static const BenchmarkItem BENCHMARKS[] = {
    { "NOP", body_nop },
    { "ADD", body_add },
    { "SUB", body_sub },
    { "INC", body_inc },
    { "DEC", body_dec },
    { "IMUL", body_imul },
    { "FMUL (x87)", body_fmul },
    { "AND", body_and },
    { "OR", body_or },
    { "XOR", body_xor },
    { "SHL", body_shl },
    { "SHR", body_shr },
    { "Arithmetic Mixed", body_arithmetic_mixed },
    { "Logic Mixed", body_logic_mixed },
    { "ADDPS (SSE)", body_sse_add_single },
    { "MULPS (SSE)", body_sse_mul_single },
    { "ADDPD (SSE)", body_sse_add_double },
    { "MULPD (SSE)", body_sse_mul_double },
    { "ANDPS (SSE)", body_sse_logic_and },
    { "ORPS (SSE)", body_sse_logic_or },
    { "XORPS (SSE)", body_sse_logic_xor },
    { "SSE Arithmetic Single Mixed", body_sse_arithmetic_single_mixed },
    { "SSE Arithmetic Double Mixed", body_sse_arithmetic_double_mixed },
    { "SSE Logic Single Mixed", body_sse_logic_single_mixed },
    { "SSE Logic Double Mixed", body_sse_logic_double_mixed },
    { "VADDPS (AVX)", body_vaddps },
    { "VMULPS (AVX)", body_vmulps },
    { "VADDPD (AVX)", body_vaddpd },
    { "VMULPD (AVX)", body_vmulpd },
    { "VANDPS (AVX)", body_vandps },
    { "VORPS (AVX)", body_vorps },
    { "VXORPS (AVX)", body_vxorps },
    { "VANDPD (AVX)", body_vandpd },
    { "VORPD (AVX)", body_vorpd },
    { "VXORPD (AVX)", body_vxorpd },
    { "AVX Arithmetic Single Mixed", body_avx_arith_single_mixed },
    { "AVX Arithmetic Double Mixed", body_avx_arith_double_mixed },
    { "AVX Logic Single Mixed", body_avx_logic_single_mixed },
    { "AVX Logic Double Mixed", body_avx_logic_double_mixed }
};
#define BENCHMARK_COUNT (sizeof(BENCHMARKS) / sizeof(BENCHMARKS[0]))