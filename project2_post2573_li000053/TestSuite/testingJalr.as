 lw 2 0 one 
 lw 3 0 thou
 lw 4 0 aAdd
 lw 5 0 bAdd
 lw 6 0 dAdd
 lw 7 0 stAdd
start beq 2 3 done
 lw 1 0 one
 add 2 1 2 
alfa jalr 5 6
bravo jalr 6 1
delta jalr 1 5
 jalr 1 7
done halt
one .fill 1
thou .fill 1000
stAdd .fill start
aAdd .fill alfa
bAdd .fill bravo
dAdd .fill delta
eAdd .fill echo
