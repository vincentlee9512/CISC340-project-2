 lw 1 0 one
 lw 2 0 one 
 lw 3 0 thou
start beq 2 3 done
 add 2 1 2 
alfa beq 0 0 fox 
bravo beq 0 0 echo
delta beq 0 0 golf
echo beq 0 0 delta
fox beq 0 0 bravo
golf beq 0 0 start
done halt
one .fill 1
thou .fill 1000
