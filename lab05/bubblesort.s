# read 10 numbers into an array
# bubblesort them
# then print the 10 numbers

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results

main:

    li   $t0, 0         # i = 0
loop0:
    bge  $t0, 10, end0  # while (i < 10) {

    li   $v0, 5         #   scanf("%d", &numbers[i]);
    syscall             #

    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    sw   $v0, ($t3)     #   store entered number in array

    addi $t0, $t0, 1    #   i++;
    j    loop0          # }
end0:
    li   $t4, 1
    
outloop:
    bne  $t4, 1, end
    
    li   $t4, 0
    li   $t0, 1
    
inloop:
    bge  $t0, 10, inend
    
    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    lw   $t5, ($t3)
    
    sub  $t1, $t0, 1    #  i - 1
    mul  $t2, $t1, 4    #  calculate &numbers[i - 1]
    la   $t3, numbers   #
    add  $t1, $t2, $t3  #
    lw   $t6, ($t1)
    
    blt  $t5, $t6, if

    add  $t0, $t0, 1
    
    j inloop
    
if:
    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    sw   $t6, ($t3)

    sub  $t1, $t0, 1    #  i - 1
    mul  $t2, $t1, 4    #  calculate &numbers[i - 1]
    la   $t3, numbers   #
    add  $t1, $t2, $t3  #
    sw   $t5, ($t1)

    li   $t4, 1

    add  $t0, $t0, 1
    
    j inloop
    
inend:
    j outloop
    
end:
    li   $t0, 0         # i = 0
    
loop1:
    bge  $t0, 10, end1  # while (i < 10) {

    mul  $t1, $t0, 4    #   calculate &numbers[i]
    la   $t2, numbers   #
    add  $t3, $t1, $t2  #
    lw   $a0, ($t3)     #   load numbers[i] into $a0
    li   $v0, 1         #   printf("%d", numbers[i])
    syscall

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    addi $t0, $t0, 1    #   i++
    j    loop1          # }
end1:

    jr   $ra            # return

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};

