global my_function

my_function:
	push ebp		; Standard function prologue
	mov ebp, esp
	mov edx, 0xb800a
	mov al, [ebp+8]	; Get first argument (from stack)
	add al, 3		; Add 3
	mov ah, 0x1f
	mov word [edx], ax

	pop ebp			; Restore previous base pointer
	ret				; Return (EAX holds the return value)


