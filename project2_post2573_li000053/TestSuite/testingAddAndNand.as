 lw 1 0 one
 lw 2 0 two
 lw 3 0 thou
start beq 2 3 done
 add 2 2 1
 add 4 1 1
 add 5 2 1
 nand 6 1 2 
 nand 7 2 1
 beq 0 0 start
done halt
one .fill 1
two .fill 2
thou .fill 1000
