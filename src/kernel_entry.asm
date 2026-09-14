BITS 32

section .data
msg_text    db "Starting kernel ...", 0

align 8
idtr:
    dw (256 * 8) - 1
    dd idt

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

align 8
idt:
    resb 256 * 8

ticks:
    resd 1

section .text
global _start
extern kmain

_start:
    cli 

    ; Segment registers reload
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_top

    ; Clear VGA text mode screen (80x25 cells)
    cld
    mov edi, 0xB8000
    mov ecx, 2000
    mov ax, 0x0F20
    rep stosw

    ; Direct VGA output for "Starting kernel ..." at top-left (0xB8000)
    mov edi, 0xB8000
    mov ecx, 19
    mov ebx, msg_text
.print_loop:
    mov al, [ebx]
    mov byte [edi], al
    mov byte [edi+1], 0x0F
    inc ebx
    add edi, 2
    dec ecx
    jnz .print_loop

    ; Initialize IDT, PIC remap, and PIT
    call init_idt
    call remap_pic
    call init_pit

    sti                         ; Enable hardware interrupts

    call kmain                  ; Jump to C kernel entry point

.hang:
    cli
    hlt
    jmp .hang

; --- IDT Setup ---
init_idt:
    push edi
    push ecx
    push edx
    push eax

    mov edi, idt
    mov ecx, 256
    mov eax, unhandled_int

    ; Populate all 256 gates with default unhandled_int handler
.fill_idt:
    mov edx, eax
    shr edx, 16
    shl edx, 16
    or edx, 0x00008E00          ; Present, ring 0, 32-bit interrupt gate

    mov bx, 0x08                ; Code selector offset
    shl ebx, 16
    mov bx, ax

    mov [edi], bx
    mov word [edi+2], 0x08
    mov byte [edi+4], 0
    mov byte [edi+5], 0x8E
    mov dx, ax
    shr eax, 16
    mov [edi+6], ax
    mov eax, unhandled_int

    add edi, 8
    dec ecx
    jnz .fill_idt

    ; Override vector 0x20 (IRQ0 / PIT)
    mov edi, idt + (0x20 * 8)
    mov eax, timer_handler
    mov [edi], ax
    shr eax, 16
    mov [edi+6], ax

    lidt [idtr]
    pop eax
    pop edx
    pop ecx
    pop edi
    ret

unhandled_int:
    iret

; --- PIC Remapping (Master 0x20-0x27, Slave 0x28-0x2F) ---
remap_pic:
    push ax
    mov al, 0x11
    out 0x20, al
    out 0xA0, al

    mov al, 0x20                ; Master offset
    out 0x21, al
    mov al, 0x28                ; Slave offset
    out 0xA1, al

    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al

    mov al, 0x01
    out 0x21, al
    out 0xA1, al

    ; Unmask only IRQ0 (timer) on master PIC (11111110b = 0xFE)
    mov al, 0xFE
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al

    pop ax
    ret

; --- PIT Configuration (~100 Hz, Channel 0, Mode 3) ---
init_pit:
    push ax
    mov al, 0x36
    out 0x43, al
    mov ax, 11932               ; 1193180 Hz / 100 Hz = 11932 (0x2E9C)
    out 0x40, al
    mov al, ah
    out 0x40, al
    pop ax
    ret

; --- IRQ0 Timer Handler ---
timer_handler:
    pushad
    inc dword [ticks]           ; Increment tick counter in memory

    ; Send End-Of-Interrupt (EOI) to Master PIC
    mov al, 0x20
    out 0x20, al

    popad
    iret