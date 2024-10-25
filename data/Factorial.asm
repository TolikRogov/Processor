in								#enter number to calculate factorial
pop ax

call fact:
push ax
out
hlt

fact:
	push ax
	pop [bx]					#write to ram ax value

	push 1
	push bx
	add
	pop bx						#next index in ram

	push ax
	push 1
	je end:						#compare ax value with 1

	push ax
	push 1
	sub
	pop ax						#decrease ax value by 1

	call fact:					#recursion call

	end:
		push bx
		push 1
		sub
		pop bx					#previous index in ram

		push ax
		push [bx]
		mul
		pop ax					#composition of ax value and value with index bx in ram
ret
