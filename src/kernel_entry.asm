; ==============================================================================
; PROJECT: 32-bit Protected Mode Kernel Stub (Loaded at 0x10000)
; FEATURE: Real-time visual progress output to the VGA text mode buffer
; ==============================================================================

[BITS 32]

section .data
msg_start   db "Starting kernel ...", 0
msg_fpu     db "Initializing FPU & SSE ...", 0
msg_idt     db "Setting up IDT and gates ...", 0
msg_pic     db "Remapping Master/Slave PIC ...", 0
msg_pit     db "Configuring PIT timer (100 Hz) ...", 0
msg_jump    db "Jumping to C kmain() ...", 0

align 8
idtr:
    dw (256 * 8) - 1                ; IDT limit (size - 1)
    dd idt                          ; IDT base linear address

section .bss
align 16
stack_bottom:
    resb 16384                      ; 16 KB kernel stack space
stack_top:

align 8
idt:
    resb 256 * 8                    ; Space for 256 interrupt descriptors (8 bytes each)

ticks:
    resd 1                          ; System tick counter

section .text
global _start
extern kmain

_start:
    cli                             ; Clear interrupts during core setup

    ; --------------------------------------------------------------------------
    ; Step 1: Reload Segment Registers and Initialize Stack
    ; --------------------------------------------------------------------------
    mov ax, 0x10                    ; Selector 0x10 is our 32-bit flat data descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_top              ; Set up kernel stack pointer

    ; 16-byte stack alignment check/force for SSE safety
    and esp, -16

    ; --------------------------------------------------------------------------
    ; Step 2: Clear Screen and Print Initial Boot Message (Row 0)
    ; --------------------------------------------------------------------------
    cld
    mov edi, 0xB8000                ; VGA text mode buffer base address
    mov ecx, 2000                   ; 80 * 25 cells = 2000 words
    mov ax, 0x0F20                  ; Attribute: White on black (0x0F), Space char (0x20)
    rep stosw                       ; Clear entire screen

    ; Print "Starting kernel ..." at row 0, column 0 (0xB8000)
    mov esi, msg_start
    xor ebx, ebx                    ; Row 0
    call print_row

    ; --------------------------------------------------------------------------
    ; Step 3: Subsystem Initializations with Live Screen Updates
    ; --------------------------------------------------------------------------
    
    ; --- FPU & SSE ---
    mov esi, msg_fpu
    mov ebx, 1                      ; Row 1
    call print_row
    call init_fpu_sse

    ; --- IDT ---
    mov esi, msg_idt
    mov ebx, 2                      ; Row 2
    call print_row
    call init_idt

    ; --- PIC ---
    mov esi, msg_pic
    mov ebx, 3                      ; Row 3
    call print_row
    call remap_pic

    ; --- PIT ---
    mov esi, msg_pit
    mov ebx, 4                      ; Row 4
    call print_row
    call init_pit

    ; --- Jump to C Kernel ---
    mov esi, msg_jump
    mov ebx, 5                      ; Row 5
    call print_row

    sti                             ; Enable hardware interrupts safely now
    call kmain                      ; Jump to C kernel entry point

.hang:
    cli
    hlt
    jmp .hang

; ==============================================================================
; Helper Function: Print a null-terminated string to a specific screen row
; Inputs: ESI = pointer to string, EBX = row number (0 to 24)
; ==============================================================================
print_row:
    push eax
    push ecx
    push edx
    push edi
    push ebx

    ; Calculate VGA memory address for row: 0xB8000 + (row * 80 * 2)
    mov edi, 0xB8000
    mov eax, ebx
    imul eax, eax, 160              ; 80 characters * 2 bytes per cell = 160 bytes per row
    add edi, eax                    ; EDI now points to the target row start

.print_char_loop:
    mov al, [esi]                   ; Load character from string
    test al, al                     ; Check for null terminator (0)
    jz .print_done                  
    mov byte [edi], al              ; Write character ASCII byte
    mov byte [edi+1], 0x0F          ; Write attribute byte: Bright white on black (0x0F)
    inc esi                         ; Next character in string
    add edi, 2                      ; Next cell in video memory
    jmp .print_char_loop

.print_done:
    pop ebx
    pop edi
    pop edx
    pop ecx
    pop eax
    ret

; ==============================================================================
; FPU & SSE Initialization (32-bit x86)
; ==============================================================================
init_fpu_sse:
    push eax
    push ecx
    push edx

    ; CR0: clear EM (bit 2), set MP (bit 1), set NE (bit 5)
    mov eax, cr0
    and eax, ~(1 << 2)
    or  eax,  (1 << 1) | (1 << 5)
    mov cr0, eax

    ; CPUID check: Check if AVX (ECX Bit 28) is supported
    mov eax, 1
    cpuid
    test ecx, (1 << 28)
    jz .no_avx

    ; CR4: set OSFXSR (bit 9), OSXMMEXCPT (bit 10), OSXSAVE (bit 18)
    mov eax, cr4
    or  eax,  (1 << 9) | (1 << 10) | (1 << 18)
    mov cr4, eax

    ; Configure XCR0: enable bits 0/1 (x87/SSE) and bit 2 (YMM) -> 0x7
    xor ecx, ecx
    xgetbv
    or eax, 0x07
    xsetbv
    jmp .fpu_done

.no_avx:
    ; Fallback: only SSE/FXSR enabled (without AVX/YMM)
    mov eax, cr4
    or  eax,  (1 << 9) | (1 << 10)
    mov cr4, eax

.fpu_done:
    fninit
    pop edx
    pop ecx
    pop eax
    ret

; ==============================================================================
; IDT Setup (Fixed 8-Byte Descriptor Construction)
; ==============================================================================
init_idt:
    push edi
    push ecx
    push eax

    mov edi, idt                    ; Destination pointer to IDT base
    mov ecx, 256                    ; Loop counter for all 256 interrupt vectors
    mov eax, unhandled_int          ; Default handler function pointer

.fill_idt:
    ; Build each 8-byte IDT descriptor cleanly:
    ; Bytes 0-1: Offset low (bits 0-15)
    mov [edi], ax
    
    ; Bytes 2-3: Code segment selector (0x08)
    mov word [edi+2], 0x08

    ; Byte 4: Reserved (must be 0)
    mov byte [edi+4], 0

    ; Byte 5: Type and attributes (Present, ring 0, 32-bit interrupt gate = 0x8E)
    mov byte [edi+5], 0x8E

    ; Bytes 6-7: Offset high (bits 16-31)
    push eax
    shr eax, 16
    mov [edi+6], ax
    pop eax

    add edi, 8                      ; Advance to next 8-byte descriptor entry
    dec ecx
    jnz .fill_idt

    ; Override vector 0x20 (IRQ0 / PIT timer interrupt) with specific handler
    mov edi, idt + (0x20 * 8)
    mov eax, timer_handler
    
    mov [edi], ax
    mov word [edi+2], 0x08
    mov byte [edi+4], 0
    mov byte [edi+5], 0x8E
    shr eax, 16
    mov [edi+6], ax

    lidt [idtr]                     ; Load IDTR with base and limit
    pop eax
    pop ecx
    pop edi
    ret

unhandled_int:
    iret

; ==============================================================================
; PIC Remapping (Master to 0x20-0x27, Slave to 0x28-0x2F)
; ==============================================================================
remap_pic:
    push ax
    mov al, 0x11                    ; Initialization command (ICW1)
    out 0x20, al
    out 0xA0, al

    mov al, 0x20                    ; Master PIC vector offset (0x20)
    out 0x21, al
    mov al, 0x28                    ; Slave PIC vector offset (0x28)
    out 0xA1, al

    mov al, 0x04                    ; Tell Master about Slave at IRQ2 (ICW3)
    out 0x21, al
    mov al, 0x02                    ; Tell Slave its cascade identity (ICW3)
    out 0xA1, al

    mov al, 0x01                    ; 8086 mode environment (ICW4)
    out 0x21, al
    out 0xA1, al

    ; Unmask only IRQ0 (timer) on master PIC (11111110b = 0xFE), mask others
    mov al, 0xFE
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al

    pop ax
    ret

; ==============================================================================
; PIT Configuration (~100 Hz, Channel 0, Mode 3 Rate Generator)
; ==============================================================================
init_pit:
    push ax
    mov al, 0x36                    ; Channel 0, Binary mode, Mode 3, LSB/MSB
    out 0x43, al
    mov ax, 11932                   ; 1193180 Hz / 100 Hz = 11932 (0x2E9C)
    out 0x40, al                    ; Send low byte
    mov al, ah
    out 0x40, al                    ; Send high byte
    pop ax
    ret

; ==============================================================================
; IRQ0 Timer Interrupt Handler
; ==============================================================================
timer_handler:
    pushad                          ; Save all general-purpose registers
    inc dword [ticks]               ; Increment system tick counter in memory

    ; Send End-Of-Interrupt (EOI) signal to Master PIC
    mov al, 0x20
    out 0x20, al

    popad                           ; Restore general-purpose registers
    iret                            ; Return from interrupt