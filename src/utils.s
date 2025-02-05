global hide_cursor

; Select the Cursor Start Register, and set it's 5th bit to one to hide the cursor
hide_cursor:
	push ebp
	mov ebp, esp

	mov dx, 0x3d4	; VGA register select port
	mov al, 0x0a	; Cursor Start Register
	out dx, al
	mov dx, 0x3d5	; VGA data port
	mov al, 0x20	; Set bit 5
	out dx, al

	pop ebp
	ret

