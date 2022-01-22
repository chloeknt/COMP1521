# COMP1521 21T2 ... final exam, question 2

	.data
even_parity_str:	.asciiz "the parity is even\n"
odd_parity_str:		.asciiz "the parity is odd\n"

	.text
main:
	li	$v0, 5
	syscall
	move	$t0, $v0
	# input is in $t0

    li  $t1, 0    # bit_idx
    li  $t2, 0    # n_bits_set
loop:
    beq $t1, 32, end
    
    srl $t3, $t0, $t1 
    and $t3, $t3, 1 
    add $t2, $t2, $t3 
    addi $t1, $t1, 1
    
    j loop

end:
    rem $t4, $t2, 2
    
if: 
    beq $t4, 0, else
    
    li	$v0, 4
	la	$a0, odd_parity_str
	syscall

	li	$v0, 0
	jr	$ra
    
else:
	li	$v0, 4
	la	$a0, even_parity_str
	syscall

	li	$v0, 0
	jr	$ra
