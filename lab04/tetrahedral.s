# Read a number n and print the first n tetrahedral numbers
# https://en.wikipedia.org/wiki/Tetrahedral_number

main:                  # int main(void) {

    la   $a0, prompt   # printf("Enter how many: ");
    li   $v0, 4
    syscall

    li   $v0, 5         # scanf("%d", number);
    syscall

    li   $t0, 1         # n = 1;
    li   $t1, 1         # i = 1;
    li   $t2, 1         # j = 1;
    li   $t3, 0         # total = 0; 
    move $t4, $v0       # $t4 = how_many;
    
loop1:
    bgt $t0, $t4, end   # if (n > how_many) goto end;
    
loop2: 
    bgt $t2, $t0, exit1 # if (j > n) goto exit1;
    
loop3:
    bgt $t1, $t2, exit2 # if (i > j) goto exit2;
    
    add $t3, $t3, $t1   # total = total + i;
    
    addi $t1, $t1, 1    # i++;
    
    j   loop3           # goto loop3

exit1:
    move $a0, $t3       # printf("%d", total);
    li $v0, 1
    syscall
    
    li $a0, '\n'        # printf('\n');
    li $v0, 11
    syscall 
    
    li   $t2, 1         # set j = 1;
    li   $t3, 0         # set total = 0; 
    addi $t0, $t0, 1    # n++;
    j   loop1 

exit2:
    li   $t1, 1         # i = 1;
    addi $t2, $t2, 1    # j++;
    j   loop2 

end:
    jr   $ra            # return

    .data
prompt:
    .asciiz "Enter how many: "
