#include <stdint.h>

// Software 64-bit division fallback for freestanding binaries
uint64_t __udivdi3(uint64_t n, uint64_t d) {
    if (d == 0) return 0;
    uint64_t q = 0, r = 0;
    for (int i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) {
            r -= d;
            q |= (1ULL << i);
        }
    }
    return q;
}

static volatile char* vga = (volatile char*)0xB8000;

// ==========================================
// Low-Level Hardware I/O
// ==========================================
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %w1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %w1" : : "a"(val), "Nd"(port));
}

// ==========================================
// VGA Text Mode Driver
// ==========================================
static void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x07;
    }
}

static void vga_print_raw(int row, int col, const char* str, uint8_t attr) {
    int offset = (row * 80 + col) * 2;
    while (*str) {
        vga[offset++] = *str++;
        vga[offset++] = attr;
    }
}

static void vga_print_num(int row, int col, uint64_t val) {
    char buf[21];
    int i = 19;
    buf[20] = '\0';
    if (val == 0) {
        buf[i--] = '0';
    } else {
        while (val > 0 && i >= 0) {
            buf[i--] = '0' + (val % 10);
            val /= 10;
        }
    }
    vga_print_raw(row, col, "                   ", 0x07);
    vga_print_raw(row, col, &buf[i + 1], 0x0A);
}

// ==========================================
// Input Handling (PS/2 Polling)
// ==========================================
#define KEY_UP         0xE048
#define KEY_DOWN       0xE050
#define KEY_ENTER      0x1C
#define KEY_NUM_START  0x02

static int get_key(void) {
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);
            if (sc == 0xE0) {
                int timeout = 100000;
                while (!(inb(0x64) & 1) && --timeout);
                if (timeout == 0) continue;
                uint8_t sc2 = inb(0x60);
                if (!(sc2 & 0x80)) return (0xE000 | sc2);
            } else if (!(sc & 0x80)) {
                return sc;
            }
        }
    }
}

// ==========================================
// RDTSC High-Resolution Timing & Unrolled Benchmarking
// ==========================================
static inline uint64_t rdtsc_start(void) {
    uint32_t lo, hi;
    __asm__ volatile (
        "cpuid\n\t"
        "rdtsc"
        : "=a"(lo), "=d"(hi)
        : 
        : "ebx", "ecx", "memory"
    );
    return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t rdtsc_end(void) {
    uint32_t lo, hi;
    __asm__ volatile (
        "rdtsc\n\t"
        "movl %%eax, %0\n\t"
        "movl %%edx, %1\n\t"
        "cpuid"
        : "=r"(lo), "=r"(hi)
        : 
        : "rax", "rbx", "rcx", "rdx", "memory"
    );
    return ((uint64_t)hi << 32) | lo;
}

#define UNROLL_FACTOR  64ULL
#define OUTER_LOOPS    1000000ULL
#define TRIAL_COUNT    20

typedef void (*BenchBody)(uint32_t loops);

typedef struct {
    const char* label;
    BenchBody body;
} BenchmarkItem;

// 64x unrolled instruction bodies
static void body_nop(uint32_t loops) {
    __asm__ volatile (
        "1:\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops) : : "cc"
    );
}

static void body_add(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to clear state
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total add instructions)
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "add $1, %%ebx\n\t" "add $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_sub(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to clear state
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total sub instructions)
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "sub $1, %%eax\n\t" "sub $1, %%ebx\n\t" "sub $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_inc(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to clear state
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total inc instructions)
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "inc %%ebx\n\t" "inc %%ecx\n\t" "inc %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_dec(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to clear state
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total dec instructions)
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "dec %%eax\n\t" "dec %%ebx\n\t" "dec %%ecx\n\t" "dec %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_imul(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to non-zero state (1) to prevent zero-absorption
        "mov $1, %%eax\n\t"
        "mov $1, %%ebx\n\t"
        "mov $1, %%ecx\n\t"
        "mov $1, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total imul instructions)
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "imul $3, %%eax\n\t" "imul $3, %%ebx\n\t" "imul $3, %%ecx\n\t" "imul $3, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_fmul(uint32_t loops) {
    __asm__ volatile (
        // Load 4 constants onto the x87 FPU stack
        "fld1\n\t"
        "fld1\n\t"
        "fld1\n\t"
        "fld1\n\t"
        "1:\n\t"
        // Mix operations across different stack slots relative to st(0) / st(i)
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "fmul %%st(1), %%st\n\t" "fmul %%st(2), %%st\n\t" "fmul %%st(3), %%st\n\t" "fmul %%st(4), %%st\n\t"
        "decl %0\n\t"
        "jnz 1b\n\t"
        // Clean up FPU stack (pop all 4 loaded values)
        "fstp %%st(0)\n\t"
        "fstp %%st(0)\n\t"
        "fstp %%st(0)\n\t"
        "fstp %%st(0)"
        : "+r"(loops)
        :
        : "cc"
    );
}

static void body_and(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers with all-bits-set pattern
        "mov $0xffffffff, %%eax\n\t"
        "mov $0xffffffff, %%ebx\n\t"
        "mov $0xffffffff, %%ecx\n\t"
        "mov $0xffffffff, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total and instructions)
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "and $3, %%eax\n\t" "and $3, %%ebx\n\t" "and $3, %%ecx\n\t" "and $3, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_or(uint32_t loops) {
    __asm__ volatile (
        // Clear/initialize registers to zero
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total or instructions)
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "or $3, %%eax\n\t" "or $3, %%ebx\n\t" "or $3, %%ecx\n\t" "or $3, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_xor(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to zero
        "xor %%eax, %%eax\n\t"
        "xor %%ebx, %%ebx\n\t"
        "xor %%ecx, %%ecx\n\t"
        "xor %%edx, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total xor instructions)
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t" "xor $3, %%ebx\n\t" "xor $3, %%ecx\n\t" "xor $3, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_shl(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to a non-zero pattern (e.g. 1)
        "mov $1, %%eax\n\t"
        "mov $1, %%ebx\n\t"
        "mov $1, %%ecx\n\t"
        "mov $1, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total shl instructions)
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shl $1, %%eax\n\t" "shl $1, %%ebx\n\t" "shl $1, %%ecx\n\t" "shl $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_shr(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to a large value (e.g. 0x80000000 or 0xffffffff)
        "mov $0x80000000, %%eax\n\t"
        "mov $0x80000000, %%ebx\n\t"
        "mov $0x80000000, %%ecx\n\t"
        "mov $0x80000000, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total shr instructions)
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shr $1, %%eax\n\t" "shr $1, %%ebx\n\t" "shr $1, %%ecx\n\t" "shr $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}
/*
static void body_vaddps(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers to zero (or seed pattern)
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t"
        "vxorps %%ymm1, %%ymm1, %%ymm1\n\t"
        "vxorps %%ymm2, %%ymm2, %%ymm2\n\t"
        "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vaddps instructions)
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vaddps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vaddps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_vmulps(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s or non-zero pattern via vpcmpeqd
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vmulps instructions)
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vmulps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_vxorps(uint32_t loops) {
    __asm__ volatile (
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vxorps instructions)
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}
*/
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
    //{ "VADDPS", body_vaddps },
    //{ "VMULPS", body_vmulps },
    //{ "VXORPS", body_vxorps }
};
#define BENCHMARK_COUNT (sizeof(BENCHMARKS) / sizeof(BENCHMARKS[0]))

static uint64_t measure_cycles_per_instruction_fixed(BenchBody body, uint64_t outer_loops) {
    __asm__ volatile("cli");
    uint64_t best_cycles = UINT64_MAX;
    for (int t = 0; t < TRIAL_COUNT; t++) {
        uint64_t start = rdtsc_start();
        body((uint32_t)outer_loops);
        uint64_t end = rdtsc_end();
        uint64_t elapsed = (end > start) ? (end - start) : 1;
        if (elapsed < best_cycles) best_cycles = elapsed;
    }
    __asm__ volatile("sti");
    uint64_t total_instructions = outer_loops * UNROLL_FACTOR;
    return (best_cycles * 100ULL) / total_instructions;
}

// ==========================================
// UI / Menu Component
// ==========================================
static void render_menu(int current_selection) {
    for (int i = 0; i < (int)BENCHMARK_COUNT; i++) {
        int row = 2 + i;
        if (i == current_selection) {
            vga_print_raw(row, 2, "> ", 0x0E);
            vga_print_raw(row, 4, BENCHMARKS[i].label, 0x0A);
        } else {
            vga_print_raw(row, 2, "  ", 0x07);
            vga_print_raw(row, 4, BENCHMARKS[i].label, 0x07);
        }
    }
}

// ==========================================
// Kernel Entry Point
// ==========================================
void kmain(void) {
    vga_clear();
    vga_print_raw(0, 2, "=== x86 RDTSC Benchmark ===", 0x0A);
    vga_print_raw(BENCHMARK_COUNT + 3, 2, "Arrow up/down: Selection | Enter: Start", 0x07);

    int current_selection = 0;
    int last_selection = -1;

    while (1) {
        if (current_selection != last_selection) {
            render_menu(current_selection);
            last_selection = current_selection;
        }

        int key = get_key();

        if (key == KEY_UP) {
            current_selection = (current_selection - 1 + BENCHMARK_COUNT) % BENCHMARK_COUNT;
        } else if (key == KEY_DOWN) {
            current_selection = (current_selection + 1) % BENCHMARK_COUNT;
        } else if (key == KEY_ENTER) {
            vga_print_raw(BENCHMARK_COUNT + 5, 2, "Running benchmark...                                        ", 0x0E);
            uint64_t cpi_scaled = measure_cycles_per_instruction_fixed(BENCHMARKS[current_selection].body, OUTER_LOOPS);
            vga_print_raw(BENCHMARK_COUNT + 5, 2, "CPI * 100, e.g. 25=0.25 (lower is better): ", 0x07);
            vga_print_num(BENCHMARK_COUNT + 5, 50, cpi_scaled);
        } else if (key >= KEY_NUM_START && key < (int)(KEY_NUM_START + BENCHMARK_COUNT)) {
            current_selection = key - KEY_NUM_START;
        }
    }
}