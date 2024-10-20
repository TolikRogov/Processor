in
pop bx

in
pop ax

push 1
pop cx

push 0
pop dx

call power:
hlt

power:
	start:
		push bx
		push cx
		mul
		pop cx

		push dx
		push 1
		add
		pop dx

		push dx
		push ax
	jb start:
	push cx
	out
ret
