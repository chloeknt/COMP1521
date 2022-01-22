# Read a number and print positive multiples of 7 or 11 < n

main:                  # int main(void) {

    la   $a0, prompt   # printf("Enter a number: ");
    li   $v0, 4
    syscall

    li   $v0, 5         # scanf("%d", number);
    syscall
    
    li   $t0, 7         # i = 7;
    move $t1, $v0       # $t1 = number;

loop:
    ble $t1, $t0, end   # if (i > number) goto end;
    
    rem $t2, $t0, 7    # if (i % 7 == 0)
    beq $t2, 0, multiple
  
    rem $t2, $t0, 11    # if (i % 11 == 0)
    beq $t2, 0, multiple
    
    addi $t0, $t0, 1    # i++;
    
    j   loop            # goto loop;

multiple:
    move $a0, $t0       # printf("%d", i);
    li $v0, 1
    syscall
    
    li $a0, '\n'        # printf('\n');
    li $v0, 11
    syscall 
    
    addi $t0, $t0, 1 
    j   loop
end:
    jr   $ra           # return

    .data

prompt:
    .asciiz "Enter a number: "
