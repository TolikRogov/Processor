in
pop ax

push ax
push 2
idiv

push ax
push 2
idiv

mul
pop ex

call circle:
draw
hlt

circle:
	loop:
	call get_x:
	call get_y:
	push ex

	push bx
	push bx
	mul

	push cx
	push cx
	mul

	add

	ja into:
	jmp end:

	into:
		push 1
		pop [dx]

	end:
		push 1
		push dx
		add
		pop dx

		push dx
		push ax
		push ax
		mul
		jb loop:
ret

get_x:
	push dx
	push ax
	mod

	push ax
	push 2
	idiv

	sub
	pop bx
ret

get_y:
	push dx
	push ax
	idiv

	push ax
	push 2
	idiv

	sub
	pop cx
ret
