#include <stdint.h>
#include "benchmarks.h"

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

// Define the maximum number of rows visible on the screen for the menu
#define MAX_VISIBLE_ROWS 20

// Keep track of the top-most visible item in the scrolling window
static int scroll_offset = 0;

// ==========================================
// UI / Menu Component with Scrolling Support
// ==========================================
static void render_menu(int current_selection) {
    // Adjust scroll offset dynamically based on current selection
    if (current_selection < scroll_offset) {
        scroll_offset = current_selection;
    } else if (current_selection >= scroll_offset + MAX_VISIBLE_ROWS) {
        scroll_offset = current_selection - MAX_VISIBLE_ROWS + 1;
    }

    // Render only the visible window of benchmarks
    for (int i = 0; i < MAX_VISIBLE_ROWS; i++) {
        int item_index = scroll_offset + i;
        int row = 2 + i;

        // Clear the line first or overwrite properly if list size varies
        if (item_index < (int)BENCHMARK_COUNT) {
            if (item_index == current_selection) {
                vga_print_raw(row, 2, ">                             ", 0x0E);
                vga_print_raw(row, 4, BENCHMARKS[item_index].label, 0x0A);
            } else {
                vga_print_raw(row, 2, "                              ", 0x07);
                vga_print_raw(row, 4, BENCHMARKS[item_index].label, 0x07);
            }
        } else {
            // Clear unused rows if the total list is smaller than MAX_VISIBLE_ROWS
            vga_print_raw(row, 2, "                                            ", 0x07);
        }
    }
}

// ==========================================
// Kernel Entry Point
// ==========================================
void kmain(void) {
    vga_clear();
    vga_print_raw(0, 2, "=== x86 RDTSC Benchmark ===", 0x0A);
    vga_print_raw(0, 40, "Arrow up/down: Selection | Enter: Start", 0x07);

    int current_selection = 0;
    int last_selection = -1;
    int last_scroll_offset = -1; // Track scroll offset changes to force redraws

    while (1) {
        // Redraw if selection or the scrolling window position changed
        if (current_selection != last_selection || scroll_offset != last_scroll_offset) {
            render_menu(current_selection);
            last_selection = current_selection;
            last_scroll_offset = scroll_offset;
        }

        int key = get_key();

        if (key == KEY_UP) {
            current_selection = (current_selection - 1 + BENCHMARK_COUNT) % BENCHMARK_COUNT;
        } else if (key == KEY_DOWN) {
            current_selection = (current_selection + 1) % BENCHMARK_COUNT;
        } else if (key == KEY_ENTER) {
            vga_print_raw(23, 2, "Running benchmark...                                                  ", 0x0E);
            uint64_t cpi_scaled = measure_cycles_per_instruction_fixed(BENCHMARKS[current_selection].body, OUTER_LOOPS);
            vga_print_raw(23, 2, "CPI * 100, e.g. 25=0.25 (lower is better): ", 0x07);
            vga_print_num(23, 60, cpi_scaled);
        } else if (key >= KEY_NUM_START && key < (int)(KEY_NUM_START + BENCHMARK_COUNT)) {
            current_selection = key - KEY_NUM_START;
        }
    }
}