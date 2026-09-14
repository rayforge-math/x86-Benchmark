BITS 16
org 0x7c00

start:
    cli                          ; Disable maskable interrupts
    xor ax, ax                   ; Zero out AX
    mov ds, ax                   ; Data segment = 0
    mov es, ax                   ; Extra segment = 0
    mov ss, ax                   ; Stack segment = 0
    mov sp, 0x7c00               ; Stack grows downward from bootloader base
    sti                          ; Re-enable interrupts

    mov [boot_drive], dl         ; Save boot drive number provided by BIOS in DL

    mov ax, 0x0003               ; Set VGA text mode 80x25 (AH=0, AL=3)
    int 0x10

    mov si, msg_boot             ; Load pointer to boot message
    call print16                 ; Print boot message

    ; LBA Read (INT 13h, AH=42h) - load 32 sectors to physical address 0x1000
    mov ah, 0x42                 ; Extended read function
    mov dl, [boot_drive]         ; Restore boot drive
    mov si, dap                  ; Point SI to Disk Address Packet (DAP)
    int 0x13                     ; Call BIOS disk service
    jc disk_error                ; Jump to error handler if carry flag is set

    mov si, msg_loaded           ; Load pointer to loaded message
    call print16                 ; Print loaded message

    ; Safe A20 activation via Fast A20 Gate (Port 0x92)
    in al, 0x92                  ; Read system control port A
    or al, 2                     ; Set bit 1 (enable A20 line)
    and al, 0xFE                 ; Clear bit 0 (avoid system reset sequence)
    out 0x92, al                 ; Write back to port 0x92

    cli                          ; Disable interrupts before switching mode
    lgdt [gdt_desc]              ; Load GDT base and limit
    mov eax, cr0                 ; Read CR0 control register
    or eax, 1                    ; Set PE (Protection Enable) bit 0
    mov cr0, eax                 ; Enable protected mode
    
    ; Far jump to 32-bit entry point at 0x1000 using code selector 0x08
    jmp 0x08:0x1000

print16:
.loop:
    lodsb                        ; Load byte at DS:SI into AL, advance SI
    or al, al                    ; Check for null terminator (0)
    jz .done                     ; If zero, finish
    mov ah, 0x0e                 ; BIOS teletype output function
    int 0x10                     ; Print character in AL
    jmp .loop                    ; Repeat for next character
.done:
    ret                          ; Return from subroutine

disk_error:
    mov si, msg_err              ; Load pointer to error message
    call print16                 ; Print error message
.h1: 
    hlt                          ; Halt CPU core
    jmp .h1                      ; Infinite loop if woken up

align 4                          ; Align DAP struct to 4-byte boundary
dap:
    db 0x10, 0                   ; DAP size (16 bytes), reserved/unused (0)
    dw 32                        ; Sector count to load (32 sectors)
    dw 0x1000                    ; Destination offset (0x1000)
    dw 0x0000                    ; Destination segment (0x0000 -> linear offset matches)
    dq 1                         ; Starting LBA sector (sector 1)

boot_drive db 0                  ; Variable holding boot drive ID
msg_boot   db 'Booting ...', 13, 10, 0
msg_loaded db 'Loaded!', 13, 10, 0
msg_err    db 'Disk Err!', 13, 10, 0

gdt_desc:
    dw gdt_end - gdt_start - 1   ; GDT limit (size - 1)
    dd gdt_start                 ; GDT linear base address
gdt_start:
    dq 0                         ; Null descriptor (required)
    dq 0x00cf9a000000ffff        ; 32-bit flat code descriptor (base 0, limit 4GB, execute/read)
    dq 0x00cf92000000ffff        ; 32-bit flat data descriptor (base 0, limit 4GB, read/write)
gdt_end:

times 510 - ($ - $$) db 0        ; Pad boot sector to 510 bytes
dw 0xAA55                        ; Standard MBR boot signature