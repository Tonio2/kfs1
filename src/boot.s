global load_gdt
global get_gdt
global get_pe
global start

extern kernel_main

; ##############################################################################

section .text

load_gdt:
	cli
	mov eax, [esp + 4]
	lgdt [eax]

	jmp 0x08:.update_cs
.update_cs:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	sti
	ret

get_gdt:
	push ebp
	mov ebp, esp

	sgdt [gdt_info]
	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]

	mov ecx, [gdt_info + 2]
	mov edx, [gdt_info]

	mov [eax], ecx
	mov [ebx], edx

	pop ebp
	ret

get_pe:
	push ebp
	mov ebp, esp

	mov eax, cr0
	mov [ebp + 8], eax

	pop ebp
	ret

start:
	mov esp, stack_top
	call kernel_main

	; disable interrupts and halt execution
	cli
.hang:
	hlt
	jmp .hang

; ##############################################################################

section .bss

align 16
gdt_info resb 6
stack_bottom:
	resb 16384	; Reserve 16 KiB for stack
stack_top:
