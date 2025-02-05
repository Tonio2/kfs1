global position_cursor

; extern void set_cursor_position(int row, int col);


; Select the Cursor Start Register, and set it's 5th bit to one to hide the cursor
position_cursor:
	push ebp
	mov ebp, esp

	mov edi, [esp + 4]	; Load the row argument (EDI)
	mov esi, [esp + 8]	; Load the column argument (ESI)

	; Set the horizontal position (column)
	mov dx, 0x3D4		; VGA register control port
	mov al, 0x0E		; Select the "Horizontal (column) Position" register
	out dx, al

	mov dx, 0x3D5		; VGA register control port
	mov ax, si			; Move the column value (ESI) to AL
	out dx, al

	; Set the vertical position (row)
	mov dx, 0x3D4		; VGA register control port
	mov al, 0x0F		; Select the "Vertical (row) Position" register
	out dx, al

	mov dx, 0x3D5		; VGA register control port
	mov ax, di			; Move the row value (EDI) to AL
	out dx, al

	pop ebp
	ret

