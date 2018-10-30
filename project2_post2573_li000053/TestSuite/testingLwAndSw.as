 lw 7 0 thou
 lw 2 0 two
start beq 2 7 done
 lw 1 0 one
 add 2 2 1
 lw 3 0 three
 lw 4 0 four
 lw 5 0 five
 lw 6 0 six
 sw 1 7 1
 sw 3 7 2
 sw 4 7 3
 sw 5 7 4
 sw 6 7 5
 sw 7 7 6
 beq 0 0 start
done halt
one .fill 1
two .fill 2
three .fill 3
four .fill 4
five .fill 5
six .fill 6
thou .fill 1000
