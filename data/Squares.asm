push 0
pop ax

call squares:
hlt

squares:
	loop:
		push ax
		push ax
		mul
		out

		push ax
		push 1
		add
		pop ax

		push ax
		push 20
	jb loop:
ret
