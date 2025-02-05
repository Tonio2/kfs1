%define MAGIC 0x1BADB002
%assign ALIGN 1
%assign MEMINFO 2
%assign FLAGS (ALIGN | MEMINFO)  ; Now NASM evaluates this to 3
%assign CHECKSUM (0x100000000 - (MAGIC + FLAGS))  ; Ensure correct computation

global _start
extern kernel_main

section .mb_header
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384  ; Reserve 16 KiB for stack
stack_top:

section .text

_start:
    mov esp, stack_top
    call kernel_main
    ;hlt
    cli
.hang:
    hlt
    jmp .hang


