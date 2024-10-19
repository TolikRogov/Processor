push 0
pop ax

call squares:
hlt

squares:
next:
push ax
push ax
mul
out

push ax
push 1
add
pop ax

push ax
push 10
jb next:
ret
