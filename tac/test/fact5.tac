r9 = 5
rax = 1
loopbegin: if r9 == 0 jump loopend
rax = rax * r9
r9 = r9 - 1
jump loopbegin
loopend: rdi = rax
