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

static void body_arithmetic_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers to a non-zero state (1) to prevent underflow issues
        "mov $1, %%eax\n\t"
        "mov $1, %%ebx\n\t"
        "mov $1, %%ecx\n\t"
        "mov $1, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains mixed with add, sub, inc, dec interleaved 16 times (= 64 total instructions)
        "add $1, %%eax\n\t" "sub $1, %%ebx\n\t" "inc %%ecx\n\t" "dec %%edx\n\t"
        "sub $1, %%eax\n\t" "add $1, %%ebx\n\t" "dec %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "dec %%ebx\n\t" "add $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "dec %%eax\n\t" "inc %%ebx\n\t" "sub $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "sub $1, %%ebx\n\t" "inc %%ecx\n\t" "dec %%edx\n\t"
        "sub $1, %%eax\n\t" "add $1, %%ebx\n\t" "dec %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "dec %%ebx\n\t" "add $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "dec %%eax\n\t" "inc %%ebx\n\t" "sub $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "sub $1, %%ebx\n\t" "inc %%ecx\n\t" "dec %%edx\n\t"
        "sub $1, %%eax\n\t" "add $1, %%ebx\n\t" "dec %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "dec %%ebx\n\t" "add $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "dec %%eax\n\t" "inc %%ebx\n\t" "sub $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "add $1, %%eax\n\t" "sub $1, %%ebx\n\t" "inc %%ecx\n\t" "dec %%edx\n\t"
        "sub $1, %%eax\n\t" "add $1, %%ebx\n\t" "dec %%ecx\n\t" "inc %%edx\n\t"
        "inc %%eax\n\t" "dec %%ebx\n\t" "add $1, %%ecx\n\t" "sub $1, %%edx\n\t"
        "dec %%eax\n\t" "inc %%ebx\n\t" "sub $1, %%ecx\n\t" "add $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_logic_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize registers with a balanced bit pattern (0x55555555)
        "mov $0x55555555, %%eax\n\t"
        "mov $0x55555555, %%ebx\n\t"
        "mov $0x55555555, %%ecx\n\t"
        "mov $0x55555555, %%edx\n\t"
        "1:\n\t"
        // 4 independent register chains mixed with and, or, xor, shl, shr interleaved 16 times (= 64 total instructions)
        "and $0x7fffffff, %%eax\n\t" "or $1, %%ebx\n\t"     "xor $3, %%ecx\n\t"     "shl $1, %%edx\n\t"
        "shr $1, %%eax\n\t"         "and $0x7fffffff, %%ebx\n\t" "or $1, %%ecx\n\t"     "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t"         "shr $1, %%ebx\n\t"         "and $0x7fffffff, %%ecx\n\t" "or $1, %%edx\n\t"
        "or $1, %%eax\n\t"          "xor $3, %%ebx\n\t"         "shr $1, %%ecx\n\t"         "and $0x7fffffff, %%edx\n\t"
        "shl $1, %%eax\n\t"         "or $1, %%ebx\n\t"          "xor $3, %%ecx\n\t"         "shr $1, %%edx\n\t"
        "and $0x7fffffff, %%eax\n\t" "shl $1, %%ebx\n\t"         "or $1, %%ecx\n\t"          "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t"         "and $0x7fffffff, %%ebx\n\t" "shl $1, %%ecx\n\t"         "or $1, %%edx\n\t"
        "or $1, %%eax\n\t"          "xor $3, %%ebx\n\t"         "and $0x7fffffff, %%ecx\n\t" "shl $1, %%edx\n\t"
        "shr $1, %%eax\n\t"         "or $1, %%ebx\n\t"          "xor $3, %%ecx\n\t"         "and $0x7fffffff, %%edx\n\t"
        "and $0x7fffffff, %%eax\n\t" "shr $1, %%ebx\n\t"         "or $1, %%ecx\n\t"          "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t"         "and $0x7fffffff, %%ebx\n\t" "shr $1, %%ecx\n\t"         "or $1, %%edx\n\t"
        "or $1, %%eax\n\t"          "xor $3, %%ebx\n\t"         "and $0x7fffffff, %%ecx\n\t" "shr $1, %%edx\n\t"
        "shl $1, %%eax\n\t"         "or $1, %%ebx\n\t"          "xor $3, %%ecx\n\t"         "and $0x7fffffff, %%edx\n\t"
        "and $0x7fffffff, %%eax\n\t" "shl $1, %%ebx\n\t"         "or $1, %%ecx\n\t"          "xor $3, %%edx\n\t"
        "xor $3, %%eax\n\t"         "and $0x7fffffff, %%ebx\n\t" "shl $1, %%ecx\n\t"         "or $1, %%edx\n\t"
        "or $1, %%eax\n\t"          "xor $3, %%ebx\n\t"         "and $0x7fffffff, %%ecx\n\t" "shr $1, %%edx\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "eax", "ebx", "ecx", "edx", "cc"
    );
}

static void body_sse_add_single(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE single-precision addition across 4 independent register chains
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "addps %%xmm0, %%xmm0\n\t" "addps %%xmm1, %%xmm1\n\t" "addps %%xmm2, %%xmm2\n\t" "addps %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_mul_single(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE single-precision multiplication across 4 independent register chains
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "mulps %%xmm0, %%xmm0\n\t" "mulps %%xmm1, %%xmm1\n\t" "mulps %%xmm2, %%xmm2\n\t" "mulps %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_add_double(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE double-precision addition across 4 independent register chains
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "addpd %%xmm0, %%xmm0\n\t" "addpd %%xmm1, %%xmm1\n\t" "addpd %%xmm2, %%xmm2\n\t" "addpd %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_mul_double(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE double-precision multiplication across 4 independent register chains
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "mulpd %%xmm0, %%xmm0\n\t" "mulpd %%xmm1, %%xmm1\n\t" "mulpd %%xmm2, %%xmm2\n\t" "mulpd %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_logic_and(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE logical AND across 4 independent register chains
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "andps %%xmm0, %%xmm0\n\t" "andps %%xmm1, %%xmm1\n\t" "andps %%xmm2, %%xmm2\n\t" "andps %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_logic_or(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE logical OR across 4 independent register chains
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "orps %%xmm0, %%xmm0\n\t" "orps %%xmm1, %%xmm1\n\t" "orps %%xmm2, %%xmm2\n\t" "orps %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static void body_sse_logic_xor(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Pure 128-bit SSE logical XOR across 4 independent register chains
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "xorps %%xmm0, %%xmm0\n\t" "xorps %%xmm1, %%xmm1\n\t" "xorps %%xmm2, %%xmm2\n\t" "xorps %%xmm3, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static inline void body_sse_arithmetic_single_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Mixed 128-bit SSE single-precision arithmetic operations (addps, subps, mulps)
        "addps %%xmm1, %%xmm0\n\t" "subps %%xmm2, %%xmm1\n\t" "mulps %%xmm3, %%xmm2\n\t" "addps %%xmm0, %%xmm3\n\t"
        "subps %%xmm1, %%xmm0\n\t" "mulps %%xmm2, %%xmm1\n\t" "addps %%xmm3, %%xmm2\n\t" "subps %%xmm0, %%xmm3\n\t"
        "mulps %%xmm1, %%xmm0\n\t" "addps %%xmm2, %%xmm1\n\t" "subps %%xmm3, %%xmm2\n\t" "mulps %%xmm0, %%xmm3\n\t"
        "addps %%xmm1, %%xmm0\n\t" "subps %%xmm2, %%xmm1\n\t" "mulps %%xmm3, %%xmm2\n\t" "addps %%xmm0, %%xmm3\n\t"
        "subps %%xmm1, %%xmm0\n\t" "mulps %%xmm2, %%xmm1\n\t" "addps %%xmm3, %%xmm2\n\t" "subps %%xmm0, %%xmm3\n\t"
        "mulps %%xmm1, %%xmm0\n\t" "addps %%xmm2, %%xmm1\n\t" "subps %%xmm3, %%xmm2\n\t" "mulps %%xmm0, %%xmm3\n\t"
        "addps %%xmm1, %%xmm0\n\t" "subps %%xmm2, %%xmm1\n\t" "mulps %%xmm3, %%xmm2\n\t" "addps %%xmm0, %%xmm3\n\t"
        "subps %%xmm1, %%xmm0\n\t" "mulps %%xmm2, %%xmm1\n\t" "addps %%xmm3, %%xmm2\n\t" "subps %%xmm0, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static inline void body_sse_arithmetic_double_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Mixed 128-bit SSE double-precision arithmetic operations (addpd, subpd, mulpd)
        "addpd %%xmm1, %%xmm0\n\t" "subpd %%xmm2, %%xmm1\n\t" "mulpd %%xmm3, %%xmm2\n\t" "addpd %%xmm0, %%xmm3\n\t"
        "subpd %%xmm1, %%xmm0\n\t" "mulpd %%xmm2, %%xmm1\n\t" "addpd %%xmm3, %%xmm2\n\t" "subpd %%xmm0, %%xmm3\n\t"
        "mulpd %%xmm1, %%xmm0\n\t" "addpd %%xmm2, %%xmm1\n\t" "subpd %%xmm3, %%xmm2\n\t" "mulpd %%xmm0, %%xmm3\n\t"
        "addpd %%xmm1, %%xmm0\n\t" "subpd %%xmm2, %%xmm1\n\t" "mulpd %%xmm3, %%xmm2\n\t" "addpd %%xmm0, %%xmm3\n\t"
        "subpd %%xmm1, %%xmm0\n\t" "mulpd %%xmm2, %%xmm1\n\t" "addpd %%xmm3, %%xmm2\n\t" "subpd %%xmm0, %%xmm3\n\t"
        "mulpd %%xmm1, %%xmm0\n\t" "addpd %%xmm2, %%xmm1\n\t" "subpd %%xmm3, %%xmm2\n\t" "mulpd %%xmm0, %%xmm3\n\t"
        "addpd %%xmm1, %%xmm0\n\t" "subpd %%xmm2, %%xmm1\n\t" "mulpd %%xmm3, %%xmm2\n\t" "addpd %%xmm0, %%xmm3\n\t"
        "subpd %%xmm1, %%xmm0\n\t" "mulpd %%xmm2, %%xmm1\n\t" "addpd %%xmm3, %%xmm2\n\t" "subpd %%xmm0, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

static inline void body_sse_logic_single_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize XMM registers with all-1s pattern via pcmpeqd
        "pcmpeqd %%xmm0, %%xmm0\n\t"
        "pcmpeqd %%xmm1, %%xmm1\n\t"
        "pcmpeqd %%xmm2, %%xmm2\n\t"
        "pcmpeqd %%xmm3, %%xmm3\n\t"
        "1:\n\t"
        // Mixed 128-bit SSE single-precision logical operations (andps, orps, xorps)
        "andps %%xmm1, %%xmm0\n\t" "orps %%xmm2, %%xmm1\n\t"  "xorps %%xmm3, %%xmm2\n\t" "andps %%xmm0, %%xmm3\n\t"
        "orps %%xmm1, %%xmm0\n\t"  "xorps %%xmm2, %%xmm1\n\t" "andps %%xmm3, %%xmm2\n\t" "orps %%xmm0, %%xmm3\n\t"
        "xorps %%xmm1, %%xmm0\n\t" "andps %%xmm2, %%xmm1\n\t" "orps %%xmm3, %%xmm2\n\t"  "xorps %%xmm0, %%xmm3\n\t"
        "andps %%xmm1, %%xmm0\n\t" "orps %%xmm2, %%xmm1\n\t"  "xorps %%xmm3, %%xmm2\n\t" "andps %%xmm0, %%xmm3\n\t"
        "orps %%xmm1, %%xmm0\n\t"  "xorps %%xmm2, %%xmm1\n\t" "andps %%xmm3, %%xmm2\n\t" "orps %%xmm0, %%xmm3\n\t"
        "xorps %%xmm1, %%xmm0\n\t" "andps %%xmm2, %%xmm1\n\t" "orps %%xmm3, %%xmm2\n\t"  "xorps %%xmm0, %%xmm3\n\t"
        "andps %%xmm1, %%xmm0\n\t" "orps %%xmm2, %%xmm1\n\t"  "xorps %%xmm3, %%xmm2\n\t" "andps %%xmm0, %%xmm3\n\t"
        "orps %%xmm1, %%xmm0\n\t"  "xorps %%xmm2, %%xmm1\n\t" "andps %%xmm3, %%xmm2\n\t" "orps %%xmm0, %%xmm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "xmm0", "xmm1", "xmm2", "xmm3", "cc"
    );
}

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

static void body_vaddpd(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers to zero
        "vxorpd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vxorpd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vxorpd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vxorpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vaddpd instructions)
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "ymm3_placeholder:\n\t" // formatting fix
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vaddpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vaddpd %%ymm3, %%ymm3, %%ymm3\n\t"
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

static void body_vmulpd(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern via vpcmpeqd
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vmulpd instructions)
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vmulpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vmulpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_vandps(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern to give the AND instruction work to do
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vandps instructions)
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_vorps(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // 4 independent register chains interleaved 16 times (= 64 total vorps instructions)
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
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

static void body_avx_arith_single_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern via vpcmpeqd
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // Interleaved single-precision add and multiply operations across 4 independent register chains
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddps %%ymm0, %%ymm0, %%ymm0\n\t" "vmulps %%ymm1, %%ymm1, %%ymm1\n\t" "vaddps %%ymm2, %%ymm2, %%ymm2\n\t" "vmulps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_avx_arith_double_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern via vpcmpeqd
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // Interleaved double-precision add and multiply operations across 4 independent register chains
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "vaddpd %%ymm0, %%ymm0, %%ymm0\n\t" "vmulpd %%ymm1, %%ymm1, %%ymm1\n\t" "vaddpd %%ymm2, %%ymm2, %%ymm2\n\t" "vmulpd %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}

static void body_avx_logic_mixed(uint32_t loops) {
    __asm__ volatile (
        // Initialize YMM registers with all-1s pattern via vpcmpeqd
        "vpcmpeqd %%ymm0, %%ymm0, %%ymm0\n\t"
        "vpcmpeqd %%ymm1, %%ymm1, %%ymm1\n\t"
        "vpcmpeqd %%ymm2, %%ymm2, %%ymm2\n\t"
        "vpcmpeqd %%ymm3, %%ymm3, %%ymm3\n\t"
        "1:\n\t"
        // Interleaved logic operations (AND, OR, XOR) across 4 independent register chains
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t"  "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t"  "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t"  "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t"  "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t"  "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t"  "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t"  "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t"  "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vorps %%ymm0, %%ymm0, %%ymm0\n\t"  "vxorps %%ymm1, %%ymm1, %%ymm1\n\t" "vandps %%ymm2, %%ymm2, %%ymm2\n\t" "vorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vxorps %%ymm0, %%ymm0, %%ymm0\n\t" "vandps %%ymm1, %%ymm1, %%ymm1\n\t" "vorps %%ymm2, %%ymm2, %%ymm2\n\t"  "vxorps %%ymm3, %%ymm3, %%ymm3\n\t"
        "vandps %%ymm0, %%ymm0, %%ymm0\n\t" "vorps %%ymm1, %%ymm1, %%ymm1\n\t" "vxorps %%ymm2, %%ymm2, %%ymm2\n\t" "vandps %%ymm3, %%ymm3, %%ymm3\n\t"
        "decl %0\n\t"
        "jnz 1b"
        : "+r"(loops)
        :
        : "ymm0", "ymm1", "ymm2", "ymm3", "cc"
    );
}