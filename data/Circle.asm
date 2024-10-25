call circle:
draw
hlt

circle:
	push ax
	push 2
	add
	pop ax

	loop:
		push 666
		pop [ax]

		call idx_up:

		push ax
		push 6
	jb loop:

	push 666
	pop [9]
	push 666
	pop [14]

	push 16
	pop ax

	column:
		push 666
		pop [ax]

		push ax
		push 7
		add
		pop ax

		push 666
		pop [ax]

		push 1
		push ax
		add
		pop ax

		push bx
		push 1
		add
		pop bx

		push bx
		push 4
	jb column:

	push 666
	pop [49]
	push 666
	pop [54]

	push 58
	pop ax

	lst_row:
		push 666
		pop [ax]

		call idx_up:

		push ax
		push 62
	jb lst_row:
ret


idx_up:
	push ax
	push 1
	add
	pop ax
ret
