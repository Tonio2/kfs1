global get_gdt
global get_pe
bits 32

section .text

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

section .bss
gdt_info resb 6
