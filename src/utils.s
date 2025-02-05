global position_cursor

DEBUG equ 0x2000


; Select the Cursor Start Register, and set it's 5th bit to one to hide the cursor
position_cursor:
	push ebp
	mov ebp, esp

	mov eax, [esp + 8]	; Load row argument
	mov ecx, 80
	mul ecx				; multiply eax by 80
	mov ecx, [esp + 12]	; Load column argument
	add ecx, eax


	; Set the horizontal position (column)
	mov dx, 0x3d4		; VGA register control port
	mov al, 0x0f		; Select the "Horizontal (column) Position" register
	out dx, al

	mov dx, 0x3d5		; VGA register control port
	mov al, cl			; Move the column value (ESI) to AL
	out dx, al
	mov byte [DEBUG], al

	; Set the vertical position (row)
	mov dx, 0x3d4		; VGA register control port
	mov al, 0x0e		; Select the "Vertical (row) Position" register
	out dx, al

	mov dx, 0x3d5		; VGA register control port
	mov al, ch			; Move the row value (EDI) to AL
	out dx, al
	mov byte [DEBUG + 4], al

	pop ebp
	ret

