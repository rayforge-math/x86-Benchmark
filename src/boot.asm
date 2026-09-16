; ==============================================================================
; Verbose 16-bit Bootloader with Protected Mode Transition
; Target Architecture: x86 (16-bit Real Mode -> 32-bit Protected Mode)
; ==============================================================================

[BITS 16]
[ORG 0x7c00]

start:
    ; --------------------------------------------------------------------------
    ; Step 1: Initialize Segment Registers and Stack
    ; --------------------------------------------------------------------------
    cli                             ; Disable maskable interrupts during setup
    xor ax, ax                      ; Clear AX register (value = 0x0000)
    mov ds, ax                      ; Set Data Segment (DS) to 0x0000
    mov es, ax                      ; Set Extra Segment (ES) to 0x0000
    mov ss, ax                      ; Set Stack Segment (SS) to 0x0000
    mov sp, 0x7c00                  ; Initialize Stack Pointer (SP) right below bootloader base
    sti                             ; Re-enable maskable interrupts

    ; Save the boot drive number provided by the BIOS in register DL
    mov [boot_drive], dl            ; Store drive ID for later disk operations

    ; --------------------------------------------------------------------------
    ; Step 2: Initialize Video Mode (VGA Text Mode 80x25)
    ; --------------------------------------------------------------------------
    mov ax, 0x0003                  ; AH = 0x00 (Set Video Mode), AL = 0x03 (80x25 text mode)
    int 0x10                        ; Call BIOS video services

    ; Print initial boot message
    mov si, msg_boot                ; Load source index with pointer to boot message
    call print_string_16            ; Invoke 16-bit teletype print routine

    ; --------------------------------------------------------------------------
    ; Step 3: Check for BIOS LBA Extensions (INT 13h, AH = 41h)
    ; --------------------------------------------------------------------------
    mov ah, 0x41                    ; BIOS extension installation check function
    mov bx, 0x55AA                  ; Magic signature required by standard
    mov dl, [boot_drive]            ; Retrieve boot drive ID
    int 0x13                        ; Call BIOS disk services
    jc error_no_lba                 ; If Carry Flag is set, extensions are not supported
    cmp bx, 0xAA55                  ; Validate returned magic signature
    jne error_no_lba                ; Jump to error handler if signature mismatches
    test cx, 1                      ; Check if bit 0 (Packet API support) is active in CX
    jz error_no_lba                 ; Jump to error handler if bit 0 is cleared

    mov si, msg_lba_ok              ; Inform user that LBA is supported
    call print_string_16

    ; --------------------------------------------------------------------------
    ; Step 4: Load Sectors from Disk via LBA Extended Read (INT 13h, AH = 42h)
    ; --------------------------------------------------------------------------
    mov ah, 0x42                    ; BIOS extended disk read function code
    mov dl, [boot_drive]            ; Ensure correct boot drive ID is set
    mov si, dap                     ; Point SI register to the Disk Address Packet (DAP)
    int 0x13                        ; Execute disk read via BIOS
    jc error_disk_read              ; Jump to disk read error handler if Carry Flag is set

    mov si, msg_loaded              ; Inform user that sectors have been loaded successfully
    call print_string_16

    ; --------------------------------------------------------------------------
    ; Step 5: Enable and Verify the A20 Line
    ; --------------------------------------------------------------------------
    call enable_and_check_a20       ; Attempt to activate A20 gate and verify functionality

    mov si, msg_a20_ok              ; Inform user that A20 gate is active
    call print_string_16

    ; --------------------------------------------------------------------------
    ; Step 6: Prepare Transition to 32-bit Protected Mode
    ; --------------------------------------------------------------------------
    mov si, msg_pmode               ; Print message indicating imminent mode switch
    call print_string_16

    cli                             ; Disable interrupts completely before loading GDT
    lgdt [gdt_desc]                 ; Load Global Descriptor Table Register (GDTR)

    mov eax, cr0                    ; Read Control Register 0 (CR0) into EAX
    or eax, 1                       ; Set Protection Enable (PE) bit (bit 0)
    mov cr0, eax                    ; Write updated value back to CR0 (Entering Protected Mode)

    ; --------------------------------------------------------------------------
    ; Step 7: Far Jump to 32-bit Code Segment
    ; --------------------------------------------------------------------------
    ; Jump to 32-bit entry point at physical address 0x10000 using code selector 0x08
    jmp dword 0x08:0x10000

; ==============================================================================
; 16-bit Teletype String Printing Subroutine
; ==============================================================================
print_string_16:
.print_loop:
    lodsb                           ; Load byte from DS:SI into AL, increment SI
    or al, al                       ; Test if AL is zero (null terminator)
    jz .print_done                  ; If zero, finish printing
    mov ah, 0x0e                    ; BIOS teletype output function (AH = 0x0E)
    int 0x10                        ; Print character stored in AL to screen
    jmp .print_loop                 ; Continue loop for next character
.print_done:
    ret                             ; Return to caller

; ==============================================================================
; A20 Gate Activation and Test Routine (Port 0x92 Method)
; ==============================================================================
enable_and_check_a20:
    ; Attempt to enable A20 using the Fast A20 Gate (Port 0x92)
    in al, 0x92                     ; Read current control byte from port 0x92
    or al, 2                        ; Set bit 1 (A20 input)
    and al, 0xFE                    ; Clear bit 0 (prevent system reset)
    out 0x92, al                    ; Write modified byte back to port 0x92

    ; Verify A20 line state by checking memory wrap-around behavior
    push ds
    push es
    xor ax, ax
    mov ds, ax                      ; DS = 0x0000
    not ax                          ; AX = 0xFFFF -> ES = 0xFFFF (High memory alias)
    mov es, ax

    mov di, 0x7DFE                  ; Offset in segment DS
    mov si, 0x7EEF                  ; Offset in segment ES (maps to same physical address if A20 is off)

    mov ax, [ds:di]                 ; Read original value
    push ax                         ; Save original value on stack
    mov word [ds:di], 0xAA55        ; Write test pattern
    mov ax, [es:si]                 ; Read from alias location
    cmp ax, 0xAA55                  ; If alias changed, A20 is still disabled (wrap-around active)
    pop ax
    mov [ds:di], ax                 ; Restore original memory value
    pop es
    pop ds
    je error_a20                    ; If values match, A20 failed to open -> trigger error

    ret                             ; Return if A20 is successfully verified

; ==============================================================================
; Error Handlers and System Halt
; ==============================================================================
error_no_lba:
    mov si, msg_err_lba
    jmp halt_system

error_disk_read:
    mov si, msg_err_disk
    jmp halt_system

error_a20:
    mov si, msg_err_a20
    jmp halt_system

halt_system:
    call print_string_16
.hang_loop:
    cli                             ; Disable interrupts
    hlt                             ; Halt CPU until next external interrupt
    jmp .hang_loop                  ; Infinite safety loop

; ==============================================================================
; Data Structures and Constants
; ==============================================================================
align 4                             ; Align DAP structure to a 4-byte boundary for safety
dap:
    db 0x10                         ; Size of DAP structure (16 bytes)
    db 0                            ; Reserved, must be 0
    dw 64                           ; Number of sectors to load (64 sectors)
    dw 0x0000                       ; Destination offset address (0x0000)
    dw 0x1000                       ; Destination segment address (0x1000 -> physical 0x10000)
    dq 1                            ; Starting LBA sector (sector 1)

boot_drive      db 0                ; Variable to store the boot drive identifier

; Status and Error Messages (Null-terminated strings with CR/LF)
msg_boot        db 'Bootloader started...', 13, 10, 0
msg_lba_ok      db 'LBA extensions verified.', 13, 10, 0
msg_loaded      db 'Sectors loaded to 0x10000.', 13, 10, 0
msg_a20_ok      db 'A20 line active.', 13, 10, 0
msg_pmode       db 'Switching to Protected Mode...', 13, 10, 0

msg_err_lba     db 'Error: LBA not supported!', 13, 10, 0
msg_err_disk    db 'Error: Disk read failed!', 13, 10, 0
msg_err_a20     db 'Error: A20 gate failed!', 13, 10, 0

; ==============================================================================
; Global Descriptor Table (GDT) Configuration
; ==============================================================================
align 8
gdt_desc:
    dw gdt_end - gdt_start - 1      ; GDT Limit (Total size of GDT minus 1)
    dd gdt_start                    ; GDT Linear Base Address

gdt_start:
    dq 0x0000000000000000           ; Null Descriptor (Mandatory first entry)
    dq 0x00cf9a000000ffff           ; 32-bit Flat Code Descriptor (Base: 0, Limit: 4GB, Exec/Read)
    dq 0x00cf92000000ffff           ; 32-bit Flat Data Descriptor (Base: 0, Limit: 4GB, Read/Write)
gdt_end:

; ==============================================================================
; Boot Sector Padding and Signature
; ==============================================================================
times 510 - ($ - $$) db 0            ; Pad remaining bytes with zeros up to 510 bytes
dw 0xAA55                           ; Standard MBR boot signature (Magic number)