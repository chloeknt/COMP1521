# read a line and print whether it is a palindrom

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li   $t0, 1
    
loop0:
    lb   $t1, line($t0)
    beq  $t1, 0, end0
    
    addi $t0, $t0, 1
    
    j loop0

end0:
    li   $t1, 0
    sub  $t2, $t0, 2
    
loop1:
    bge  $t1, $t2, end1

    lb   $t3, line($t1)
    lb   $t4, line($t2)
    beq  $t3, $t4, else

if:
    la   $a0, not_palindrome
    li   $v0, 4
    syscall
    
    j return 
    
else:
    addi $t1, $t1, 1
    sub  $t2, $t2, 1
    
    j loop1

end1:
    la   $a0, palindrome
    li   $v0, 4
    syscall

return:
    li   $v0, 0          # return 0
    jr   $ra


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256

