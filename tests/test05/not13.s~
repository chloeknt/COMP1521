main:
    li   $v0, 5         #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li   $v0, 5         #   scanf("%d", &y);
    syscall             #
    move $t1, $v0

    add  $t2, $t0, 1    # i = x + 1;
    
loop:
    bge  $t2, $t1, end  # i >= y, end loop
    
    beq  $t2, 13, equal # if i == 13, jump to equal
    
    move $a0, $t2       #   printf("%d\n", i);
    li   $v0, 1
    syscall
    
    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall
    
    add  $t2, $t2, 1
    
    j loop

equal:   
    
    add $t2, $t2, 1     # i++;
    
    j loop
end:

    li   $v0, 0         # return 0
    jr   $ra
