call circle:
hlt

circle:
	loop:
		push xx
		push 32
		jb next:

		push 10
		pop [xx]

		next:
			push 1
			push xx
			add
			pop xx

		push xx
		push 64
	jb loop:
ret
