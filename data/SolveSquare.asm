in
pop ax

in
pop bx

in
pop cx

call ss:

push gx
out
push gx
push 2
jne one_rts:
two_roots:
	push ex
	out
	push fx
	out
	hlt
one_rts:
	push gx
	push 1
	jne finish:
	push ex
	out hlt
finish:
	hlt

ss:
	push ax
	push 0
	jne anz:

	call a_zero:
	ret

	anz:
	push 2
	pop gx

	push bx
	push 0
	jne anz_bz:

	push cx
	push 0
	jne anz_bz:

	push 1
	pop gx

	anz_bz:
	push bx
	push bx
	mul

	push 4
	push ax
	mul

	push cx
	mul

	sub
	sqrt

	pop dx

	call I_x:
	call II_x:
ret

I_x:
	push bx
	push -1
	mul

	push dx
	add

	push 2
	push ax
	mul

	div
	pop ex
ret

II_x:
	push bx
	push -1
	mul

	push dx
	sub

	push 2
	push ax
	mul

	div
	pop fx
ret

a_zero:
	push bx
	push 0
	jne bnz:

	call b_zero:
	ret

	bnz:
	push cx
	push -1
	mul

	push bx
	div
	pop ex
	push 1
	pop gx
ret

b_zero:
	push cx
	push 0
	jne cnz:

	push -1
	pop gx

	ret

	cnz:
	push 0
	pop gx
ret
