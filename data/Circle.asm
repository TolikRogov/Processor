call circle:
draw
hlt

circle:
	push xx
	push 2
	add
	pop xx

	loop:
		push 666
		pop [xx]

		call idx_up:

		push xx
		push 6
	jb loop:

	push 666
	pop [9]
	push 666
	pop [14]

	push 16
	pop xx

	column:
		push 666
		pop [xx]

		push xx
		push 7
		add
		pop xx

		push 666
		pop [xx]

		push 1
		push xx
		add
		pop xx

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
	pop xx

	lst_row:
		push 666
		pop [xx]

		call idx_up:

		push xx
		push 62
	jb lst_row:
ret


idx_up:
	push xx
	push 1
	add
	pop xx
ret
