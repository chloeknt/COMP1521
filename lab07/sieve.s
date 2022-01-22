# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:
    li $t0, 0
    
loop0:
    bge $t0, 1000, end0
     
    li  $t3, 1    
    sb  $t3, prime($t0) # store 1 at address
    
    addi $t0, $t0, 1    # i++
    
    j loop0
    
end0:
    li $t0, 2           # i = 2

loop1:
    bge $t0, 1000, end2 
     
    li  $t3, 1    
    lb  $t3, prime($t0) # store value at address
    bne $t3, 1, end1    # if (prime[i])
   
if: 
    move $a0, $t0       # printf("%d", array[x][y])
    li $v0, 1
    syscall
    
    li  $a0, '\n'       # printf("%c", '\n');
    li  $v0, 11
    syscall
    
    mul $t4, $t0, 2     # int j = 2 * i

loop2:
    bge $t4, 1000, end1
    
    li  $t3, 0    
    sb  $t3, prime($t4)  # store 0 at address
    
    add $t4, $t4, $t0
    
    j loop2

end1:
    addi $t0, $t0, 1    # i++

    j loop1
    
end2:
    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000
