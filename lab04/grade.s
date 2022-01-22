# read a mark and print the corresponding UNSW grade

main:
    la   $a0, prompt    # printf("Enter a mark: ");
    li   $v0, 4
    syscall

    li   $v0, 5         # scanf("%d", mark);
    syscall
    
    slt $t0, $v0, 50    # if (mark < 50)
    beq $t0, 1, fail
    
    slt $t0, $v0, 65    # if (mark < 65)
    beq $t0, 1, pass
    
    slt $t0, $v0, 75    # if (mark < 75)
    beq $t0, 1, credit
    
    slt $t0, $v0, 85    # if (mark < 85)
    beq $t0, 1, distinction
  
    la $a0, hd
    li $v0, 4
    syscall
    
    j   end 
    
fail:
    la $a0, fl
    li $v0, 4
    syscall
    
    j   end
        
pass:
    la $a0, ps
    li $v0, 4
    syscall
    
    j   end
    
credit:
    la $a0, cr
    li $v0, 4
    syscall
    
    j   end 

distinction:
    la $a0, dn
    li $v0, 4
    syscall
    
    j   end 

end:
    jr   $ra            # return

    .data
prompt:
    .asciiz "Enter a mark: "
fl:
    .asciiz "FL\n"
ps:
    .asciiz "PS\n"
cr:
    .asciiz "CR\n"
dn:
    .asciiz "DN\n"
hd:
    .asciiz "HD\n"
