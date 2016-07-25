r9 = 0
rax = 0
loopbegin: if r9 >= 10 jump loopend
r9 = r9 + 1
rax = rax + r9
jump loopbegin
loopend: rdi = rax
