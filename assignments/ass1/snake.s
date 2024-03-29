########################################################################
# COMP1521 21T2 -- Assignment 1 -- Snake!
# <https://www.cse.unsw.edu.au/~cs1521/21T2/assignments/ass1/index.html>
#
#
# !!! IMPORTANT !!!
# Before starting work on the assignment, make sure you set your tab-width to 8!
# For instructions, see: https://www.cse.unsw.edu.au/~cs1521/21T2/resources/mips-editors.html
# !!! IMPORTANT !!!
#
#
# This program was written by Chloe Toh (z5362296)
# on 28/06/2021
#
# Version 1.0 (2021-06-24): Team COMP1521 <cs1521@cse.unsw.edu.au>
#

	# Requires:
	# - [no external symbols]
	#
	# Provides:
	# - Global variables:
	.globl	symbols
	.globl	grid
	.globl	snake_body_row
	.globl	snake_body_col
	.globl	snake_body_len
	.globl	snake_growth
	.globl	snake_tail

	# - Utility global variables:
	.globl	last_direction
	.globl	rand_seed
	.globl  input_direction__buf

	# - Functions for you to implement
	.globl	main
	.globl	init_snake
	.globl	update_apple
	.globl	move_snake_in_grid
	.globl	move_snake_in_array

	# - Utility functions provided for you
	.globl	set_snake
	.globl  set_snake_grid
	.globl	set_snake_array
	.globl  print_grid
	.globl	input_direction
	.globl	get_d_row
	.globl	get_d_col
	.globl	seed_rng
	.globl	rand_value


########################################################################
# Constant definitions.

N_COLS          = 15
N_ROWS          = 15
MAX_SNAKE_LEN   = N_COLS * N_ROWS

EMPTY           = 0
SNAKE_HEAD      = 1
SNAKE_BODY      = 2
APPLE           = 3

NORTH       = 0
EAST        = 1
SOUTH       = 2
WEST        = 3


########################################################################
# .DATA
	.data

# const char symbols[4] = {'.', '#', 'o', '@'};
symbols:
	.byte	'.', '#', 'o', '@'

	.align 2
# int8_t grid[N_ROWS][N_COLS] = { EMPTY };
grid:
	.space	N_ROWS * N_COLS

	.align 2
# int8_t snake_body_row[MAX_SNAKE_LEN] = { EMPTY };
snake_body_row:
	.space	MAX_SNAKE_LEN

	.align 2
# int8_t snake_body_col[MAX_SNAKE_LEN] = { EMPTY };
snake_body_col:
	.space	MAX_SNAKE_LEN

# int snake_body_len = 0;
snake_body_len:
	.word	0

# int snake_growth = 0;
snake_growth:
	.word	0

# int snake_tail = 0;
snake_tail:
	.word	0

# Game over prompt, for your convenience...
main__game_over:
	.asciiz	"Game over! Your score was "


########################################################################
#
# Your journey begins here, intrepid adventurer!
#
# Implement the following 6 functions, and check these boxes as you
# finish implementing each function
#
#  - [X] main
#  - [X] init_snake
#  - [X] update_apple
#  - [X] update_snake
#  - [X] move_snake_in_grid
#  - [X] move_snake_in_array
#



########################################################################
# .TEXT <main>
	.text
main:

	# Args:     void
	# Returns:
	#   - $v0: int
	#
	# Frame:    $ra
	# Uses:	    [$t8. $t9, $t0, $t2,]
	# Clobbers: [$t0, $t2]
	#
	# Locals:
	#   - [int direction in $t8]
	#   - [int score in $t9]
	#
	# Structure:
	#   main
	#   -> [prologue]
	#   -> body
	#   -> loop0
	#   -> end0
	#   -> [epilogue]

	# Code:
main__prologue:
	# set up stack frame
	addiu	$sp, $sp, -4
	sw	$ra, ($sp)

main__body:
        jal     init_snake      # call init_snake
        jal     update_apple    # call update apple
        
loop0:  
        jal     print_grid      # call print_grid 
        jal     input_direction # call input_direction
        move    $t8, $v0        # direction = function return 

        move    $a0, $t8
        jal     update_snake    # call update_snake
        move    $t0, $v0     
        beq     $t0, 0, end0    # if return = 0, jump to end0
        
        j loop0 

end0:
        la      $t0, snake_body_len 
        lw      $t2, ($t0)
        div     $t9, $t2, 3     # score = snake_body_len / 3
        
        la      $a0, main__game_over   # printf ("Game over! Your score was ")
        li      $v0, 4
        syscall
        
        move    $a0, $t9        # printf("%d ", score)
        li      $v0, 1
        syscall
        
        li      $a0, '\n'       # printf("%c", '\n');
        li      $v0, 11
        syscall
       
        
main__epilogue:
	# tear down stack frame
	lw	$ra, ($sp)
	addiu 	$sp, $sp, 4

	li	$v0, 0
	jr	$ra		# return 0;



########################################################################
# .TEXT <init_snake>
	.text
init_snake:

	# Args:     void
	# Returns:  void
	#
	# Frame:    $ra
	# Uses:     [$t0, $t1]
	# Clobbers: [$t0, $t1]
	#
	# Locals:
	#   - nil
	#
	# Structure:
	#   init_snake
	#   -> [prologue]
	#   -> body
	#   -> [epilogue]

	# Code:
init_snake__prologue:
	# set up stack frame
	addiu	$sp, $sp, -4
	sw	$ra, ($sp)

init_snake__body:
        li $t0, 7

        move $a0, $t0
        move $a1, $t0
        li $a2, SNAKE_HEAD      # set_snake(7, 7, SNAKE_HEAD);
	jal set_snake           # call set_snake function
	
	li $t0, 7
	li $t1, 6
	
	move $a0, $t0
        move $a1, $t1
        li $a2, SNAKE_BODY      # set_snake(7, 6, SNAKE_BODY);
	jal set_snake           # call set_snake function
	
	li $t0, 7
	li $t1, 5
	
	move $a0, $t0
        move $a1, $t1
        li $a2, SNAKE_BODY      # set_snake(7, 5, SNAKE_BODY);
	jal set_snake           # call set_snake function
	
	li $t0, 7
        li $t1, 4
	
	move $a0, $t0
        move $a1, $t1
        li $a2, SNAKE_BODY      # set_snake(7, 4, SNAKE_BODY);
	jal set_snake           # call set_snake function

init_snake__epilogue:
	# tear down stack frame
	lw	$ra, ($sp)
	addiu 	$sp, $sp, 4

	jr	$ra		# return;



########################################################################
# .TEXT <update_apple>
	.text
update_apple:

	# Args:     void
	# Returns:  void
	#
	# Frame:    $ra
	# Uses:     [$t0, $t1, $t2, $t3, $t4, $t5]
	# Clobbers: [$t4, $t5]
	#
	# Locals:
	#   - [int apple_row in $t0]
	#   - [int apple_col in $t1]
	#
	# Structure:
	#   update_apple
	#   -> [prologue]
	#   -> body
	#   -> end1
	#   -> [epilogue]

	# Code:
update_apple__prologue:
	# set up stack frame
	addiu	$sp, $sp, -4
	sw	$ra, ($sp)

update_apple__body:        
	li $a0, N_ROWS          # rand_value(N_ROWS);
	jal rand_value
	move $t2, $v0           # apple_row = function return
	
	li $a0, N_COLS          # rand_value(N_COLS);
	jal rand_value
        move $t3, $v0           # apple_col = function return 
        
        move $t0, $t2           # store value in $t0
        move $t1, $t3           # store value in $t1
        
        mul $t4, $t2, N_COLS    # calculate &grid[apple_row][apple_col] 
        add $t4, $t4, $t3      
        la  $t3, grid          
        add $t2, $t3, $t4      
        lb  $t5, ($t2)          # load value at address
        
        beq $t5, EMPTY, end1    # if the value = EMPTY jump to end
        
	j update_apple__body    # otherwise jump to start of loop
	
end1:	                        # place the apple in new location
        mul $t4, $t0, N_COLS    # calculate &grid[apple_row][apple_col] 
        add $t4, $t4, $t1       
        la  $t3, grid          
        add $t2, $t3, $t4     
        li  $t5, APPLE
        sb  $t5, ($t2)          # let grid[apple_row][apple_col] = APPLE
        
update_apple__epilogue:
	# tear down stack frame
	lw	$ra, ($sp)
	addiu 	$sp, $sp, 4

	jr	$ra		# return;



########################################################################
# .TEXT <update_snake>
	.text
update_snake:

	# Args:
	#   - $a0: int direction
	# Returns:
	#   - $v0: bool
	#
	# Frame:    $ra, $a0, $t8
	# Uses:     $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7
	# Clobbers: $t0, $t1, $t2, $t3 
	#
	# Locals:
	#   - [int d_row in $t0]
        #   - [int d_col in $t1]
	#   - [int head_row in $t5]
	#   - [int head_col in $t6]
	#   - [int new_head_row in $t2]
	#   - [int new_head_col in $t3]
	#
	# Structure:
	#   update_snake
	#   -> [prologue]
	#   -> body
	#   -> apple
	#   -> false0
	#   -> [epilogue]

	# Code:
update_snake__prologue:
	# set up stack frame
	addiu	$sp, $sp, -8
	sw	$ra, 4($sp)
	sw      $a0, 0($sp)

update_snake__body:
	move $a0, $a0 
	jal  get_d_row
	move $t0, $v0           # d_row = get_d_row(direction)
	
	move $a0, $a0
	jal  get_d_col
	move $t1, $v0           # d_col = get_d_col(direction)
		
        la   $t3, snake_body_row   
        lb   $t5, ($t3)         # load value at snake_body_row[0]
        
        la   $t3, snake_body_col    
        lb   $t6, ($t3)         # load value at snake_body_col[0]

        mul $t2, $t5, N_COLS    # calculate &grid[head_row][head_col]
        add $t2, $t2, $t6      
        la  $t3, grid          
        add $t2, $t3, $t2    
        li  $t3, SNAKE_BODY
        sb  $t3, ($t2)          # grid[head_row][head_col] = SNAKE_BODY;
        
	add $t2, $t5, $t0       # new_head_row = head_row + d_row
	add $t3, $t6, $t1       # new_head_col = head_col + d_col

        blt $t2, 0, false0      # if new_head_row < 0
	bge $t2, N_ROWS, false0 # if new_head_row >= N_ROWS
	blt $t3, 0, false0      # if new_head_col < 0
	bge $t3, N_COLS, false0 # if new_head_col >= N_COLS
                                # jump to false (out of bounds)
                                
        mul $t0, $t2, N_COLS    # calculate &grid[apple_row][apple_col]
        add $t0, $t0, $t3     
        la  $t1, grid       
        add $t0, $t1, $t0    
        lb  $t1, ($t0)          # load value at grid[apple_row][apple_col]
        
        seq $t1, $t1, APPLE     # if value = APPLE, apple = 1, otherwise 0
        
        la   $t0, snake_body_len
        lw   $t4, ($t0)
        sub  $t5, $t4, 1        # snake_body_len - 1
        la   $t0, snake_tail
        sw   $t5, ($t0)         # snake_tail = snake_body_len - 1
        
        move $a0, $t2 
        move $a1, $t3
        jal move_snake_in_grid  # move_snake_in_grid(new_head_row, new_head_col)
        move $t0, $v0           
        beq $t0, 0, false0      # if function return = 0, jump to false
        
        move $a0, $t2           
        move $a1, $t3
        jal move_snake_in_array # move_snake_in_array(new_head_row, new_head_col)
        
        beq $t1, 1, apple       # if apple = 1, jump to apple
        
        j update_snake__epilogue # otherwise jump to epilogue
        
apple:                          # apple is eaten
        la  $t0, snake_growth   
        lw  $t7, ($t0)
        add $t7, $t7, 3
        sw  $t7, ($t0)          # snake_growth increases by 3 
        
        jal update_apple        # new apple location 
        
        j update_snake__epilogue

false0:                         # prompts a game over
        # tear down stack frame
	lw      $t8, 0($sp)     # restore direction register 
	lw	$ra, 4($sp)
	addiu 	$sp, $sp, 8
	
	li	$v0, 0
	jr	$ra		# return false;
	
update_snake__epilogue:
	# tear down stack frame
	lw      $t8, 0($sp)
	lw	$ra, 4($sp)
	addiu 	$sp, $sp, 8

	li	$v0, 1
	jr	$ra		# return true;



########################################################################
# .TEXT <move_snake_in_grid>
	.text
move_snake_in_grid:

	# Args:
	#   - $a0: new_head_row
	#   - $a1: new_head_col
	# Returns:
	#   - $v0: bool
	#
	# Frame:    $ra, $a1, $a0, $t1, $t2, $t3
	# Uses:     [$t0, $t1, $t2, $t4, $t5]
	# Clobbers: [$t2]
	#
	# Locals:
	#   - [int tail in $]
	#   - [int tail_row in $]
	#   - [int tail_col in $]
	#
	# Structure:
	#   move_snake_in_grid
	#   -> [prologue]
	#   -> body
	#   -> else0
	#   -> if1
	#   -> false1
	#   -> [epilogue]

	# Code:
move_snake_in_grid__prologue:
	# set up stack frame
	addiu	$sp, $sp, -16
        sw   $ra, 12($sp)       # save $ra on $stack
        sw   $a1, 8($sp)   
        sw   $a0, 4($sp)
        sw   $t1, 0($sp)

move_snake_in_grid__body:
        la  $t1, snake_growth   # check if snake has eaten apple
        lw  $t0, ($t1)
	ble $t0, 0, else0       # if snake_growth <= 0, jump to else
        
        la  $t1, snake_tail     # increase tail by 1
        lw  $t0, ($t1)
        add $t0, $t0, 1
        sw  $t0, ($t1)
        
        la  $t1, snake_body_len # increase snake_body_len by 1
        lw  $t0, ($t1)
        add $t0, $t0, 1
        sw  $t0, ($t1)
        
        la  $t1, snake_growth   # decrease snake_growth by 1
        lw  $t0, ($t1)
        sub $t0, $t0, 1
        sw  $t0, ($t1)

        j if1                   # jump to if1
else0:
        la $t1, snake_tail      # tail = snake_tail
        lw $t0, ($t1)
        
        lb   $t4, snake_body_row($t0)     # tail_row = snake_body_row[tail];
        lb   $t5, snake_body_col($t0)     # tail_row = snake_body_col[tail];

        mul $t1, $t4, N_COLS    # calculate &grid[tail_row][tail_col]
        add $t1, $t1, $t5     
        la  $t2, grid         
        add $t1, $t2, $t1    
        li  $t2, EMPTY
        sb  $t2, ($t1)          # grid[tail_row][tail_col] = EMPTY

if1:                            # if snake runs into own body
        move $t4, $a0           # new_head_row
        move $t5, $a1           # new_head_col
        
        mul $t1, $t4, N_COLS    # calculate &grid[new_head_row][new_head_col]
        add $t1, $t1, $t5     
        la  $t2, grid        
        add $t1, $t2, $t1   
        lb  $t2, ($t1)     
        beq $t2, SNAKE_BODY, false1 # if grid[new_head_row][new_head_col] = SNAKE BODY, jump to false 
            
        li  $t2, SNAKE_HEAD
        sb  $t2, ($t1)          # grid[new_head_row][new_head_col] = SNAKE_HEAD 
        
        j move_snake_in_grid__epilogue # jump to epilogue
        
false1:                         # prompts a game over
        lw      $t1, 0($sp)     # restore variables before 
        lw      $t2, 4($sp)     # jumping back to call address
        lw      $t3, 8($sp)    
        lw      $ra, 12($sp)
	addiu   $sp, $sp, 16
	li	$v0, 0
	jr	$ra		# return false;

move_snake_in_grid__epilogue:
        lw      $t1, 0($sp)     # restore variables before 
        lw      $t2, 4($sp)     # jumping back to call address
	lw      $t3, 8($sp)    
        lw      $ra, 12($sp)
	addiu   $sp, $sp, 16

	li	$v0, 1
	jr	$ra		# return true;



########################################################################
# .TEXT <move_snake_in_array>
	.text
move_snake_in_array:

	# Args:
	#   - $a0: int new_head_row
	#   - $a1: int new_head_col
	# Returns:  void
	#
	# Frame:    $ra, $a0, $a1, $t1, $t2, $t3
	# Uses:     $t0, $t1, $t2, $t3, $t6, $t7
	# Clobbers: $t6, $t7
	#
	# Locals:
	#   - int i in $t0
	#
	# Structure:
	#   move_snake_in_array
	#   -> [prologue]
	#   -> body
	#   -> loop1
	#   -> end2
	#   -> [epilogue]

	# Code:
move_snake_in_array__prologue:
	# set up stack frame
	addiu	$sp, $sp, -16
        sw   $ra, 12($sp)       # save $ra on $stack
        sw   $a1, 8($sp)   
        sw   $a0, 4($sp)
        sw   $t1, 0($sp)

move_snake_in_array__body:
        move $t1, $a0           # new_head_row
        move $t2, $a1           # new_head_col

        la $t3, snake_tail      # i = snake_tail
	lw $t0, ($t3) 
	
loop1: 
	blt $t0, 1, end2        # if i < 1, jump to end0
	
	sub  $t3, $t0, 1        # i - 1
        lb   $t6, snake_body_row($t3)    # calculate &snake_body_row[i - 1]
	move $a0, $t6
	
        lb   $t7, snake_body_col($t3)    # calculate &snake_body_col[i - 1] 
	move $a1, $t7
	
	move $a2, $t0
	
	jal set_snake_array     # set_snake_array(snake_body_row[i - 1], snake_body_col[i - 1], i)
	
	sub $t0, $t0, 1         # i = i - 1
	
        j loop1                 # jump back to loop1
end2:
        move $a0, $t1           
        move $a1, $t2
        li $a2, 0
        jal set_snake_array     # set_snake_array(new_head_row, new_head_col, 0)
        
move_snake_in_array__epilogue:
        lw      $t1, 0($sp)     # restore variables before
        lw      $t2, 4($sp)     # returning to call address
	lw      $t3, 8($sp)     
        lw      $ra, 12($sp)
	addiu   $sp, $sp, 16

	jr	$ra		# return;


########################################################################
####                                                                ####
####        STOP HERE ... YOU HAVE COMPLETED THE ASSIGNMENT!        ####
####                                                                ####
########################################################################

##
## The following is various utility functions provided for you.
##
## You don't need to modify any of the following.  But you may find it
## useful to read through --- you'll be calling some of these functions
## from your code.
##

	.data

last_direction:
	.word	EAST

rand_seed:
	.word	0

input_direction__invalid_direction:
	.asciiz	"invalid direction: "

input_direction__bonk:
	.asciiz	"bonk! cannot turn around 180 degrees\n"

	.align	2
input_direction__buf:
	.space	2



########################################################################
# .TEXT <set_snake>
	.text
set_snake:

	# Args:
	#   - $a0: int row
	#   - $a1: int col
	#   - $a2: int body_piece
	# Returns:  void
	#
	# Frame:    $ra, $s0, $s1
	# Uses:     $a0, $a1, $a2, $t0, $s0, $s1
	# Clobbers: $t0
	#
	# Locals:
	#   - `int row` in $s0
	#   - `int col` in $s1
	#
	# Structure:
	#   set_snake
	#   -> [prologue]
	#   -> body
	#   -> [epilogue]

	# Code:
set_snake__prologue:
	# set up stack frame
	addiu	$sp, $sp, -12
	sw	$ra, 8($sp)
	sw	$s0, 4($sp)
	sw	$s1,  ($sp)

set_snake__body:
	move	$s0, $a0		# $s0 = row
	move	$s1, $a1		# $s1 = col

	jal	set_snake_grid		# set_snake_grid(row, col, body_piece);

	move	$a0, $s0
	move	$a1, $s1
	lw	$a2, snake_body_len
	jal	set_snake_array		# set_snake_array(row, col, snake_body_len);

	lw	$t0, snake_body_len
	addiu	$t0, $t0, 1
	sw	$t0, snake_body_len	# snake_body_len++;

set_snake__epilogue:
	# tear down stack frame
	lw	$s1,  ($sp)
	lw	$s0, 4($sp)
	lw	$ra, 8($sp)
	addiu 	$sp, $sp, 12

	jr	$ra			# return;



########################################################################
# .TEXT <set_snake_grid>
	.text
set_snake_grid:

	# Args:
	#   - $a0: int row
	#   - $a1: int col
	#   - $a2: int body_piece
	# Returns:  void
	#
	# Frame:    None
	# Uses:     $a0, $a1, $a2, $t0
	# Clobbers: $t0
	#
	# Locals:   None
	#
	# Structure:
	#   set_snake
	#   -> body

	# Code:
	li	$t0, N_COLS
	mul	$t0, $t0, $a0		#  15 * row
	add	$t0, $t0, $a1		# (15 * row) + col
	sb	$a2, grid($t0)		# grid[row][col] = body_piece;

	jr	$ra			# return;



########################################################################
# .TEXT <set_snake_array>
	.text
set_snake_array:

	# Args:
	#   - $a0: int row
	#   - $a1: int col
	#   - $a2: int nth_body_piece
	# Returns:  void
	#
	# Frame:    None
	# Uses:     $a0, $a1, $a2
	# Clobbers: None
	#
	# Locals:   None
	#
	# Structure:
	#   set_snake_array
	#   -> body

	# Code:
	sb	$a0, snake_body_row($a2)	# snake_body_row[nth_body_piece] = row;
	sb	$a1, snake_body_col($a2)	# snake_body_col[nth_body_piece] = col;

	jr	$ra				# return;



########################################################################
# .TEXT <print_grid>
	.text
print_grid:

	# Args:     void
	# Returns:  void
	#
	# Frame:    None
	# Uses:     $v0, $a0, $t0, $t1, $t2
	# Clobbers: $v0, $a0, $t0, $t1, $t2
	#
	# Locals:
	#   - `int i` in $t0
	#   - `int j` in $t1
	#   - `char symbol` in $t2
	#
	# Structure:
	#   print_grid
	#   -> for_i_cond
	#     -> for_j_cond
	#     -> for_j_end
	#   -> for_i_end

	# Code:
	li	$v0, 11			# syscall 11: print_character
	li	$a0, '\n'
	syscall				# putchar('\n');

	li	$t0, 0			# int i = 0;

print_grid__for_i_cond:
	bge	$t0, N_ROWS, print_grid__for_i_end	# while (i < N_ROWS)

	li	$t1, 0			# int j = 0;

print_grid__for_j_cond:
	bge	$t1, N_COLS, print_grid__for_j_end	# while (j < N_COLS)

	li	$t2, N_COLS
	mul	$t2, $t2, $t0		#                             15 * i
	add	$t2, $t2, $t1		#                            (15 * i) + j
	lb	$t2, grid($t2)		#                       grid[(15 * i) + j]
	lb	$t2, symbols($t2)	# char symbol = symbols[grid[(15 * i) + j]]

	li	$v0, 11			# syscall 11: print_character
	move	$a0, $t2
	syscall				# putchar(symbol);

	addiu	$t1, $t1, 1		# j++;

	j	print_grid__for_j_cond

print_grid__for_j_end:

	li	$v0, 11			# syscall 11: print_character
	li	$a0, '\n'
	syscall				# putchar('\n');

	addiu	$t0, $t0, 1		# i++;

	j	print_grid__for_i_cond

print_grid__for_i_end:
	jr	$ra			# return;



########################################################################
# .TEXT <input_direction>
	.text
input_direction:

	# Args:     void
	# Returns:
	#   - $v0: int
	#
	# Frame:    None
	# Uses:     $v0, $a0, $a1, $t0, $t1
	# Clobbers: $v0, $a0, $a1, $t0, $t1
	#
	# Locals:
	#   - `int direction` in $t0
	#
	# Structure:
	#   input_direction
	#   -> input_direction__do
	#     -> input_direction__switch
	#       -> input_direction__switch_w
	#       -> input_direction__switch_a
	#       -> input_direction__switch_s
	#       -> input_direction__switch_d
	#       -> input_direction__switch_newline
	#       -> input_direction__switch_null
	#       -> input_direction__switch_eot
	#       -> input_direction__switch_default
	#     -> input_direction__switch_post
	#     -> input_direction__bonk_branch
	#   -> input_direction__while

	# Code:
input_direction__do:
	li	$v0, 8			# syscall 8: read_string
	la	$a0, input_direction__buf
	li	$a1, 2
	syscall				# direction = getchar()

	lb	$t0, input_direction__buf

input_direction__switch:
	beq	$t0, 'w',  input_direction__switch_w	# case 'w':
	beq	$t0, 'a',  input_direction__switch_a	# case 'a':
	beq	$t0, 's',  input_direction__switch_s	# case 's':
	beq	$t0, 'd',  input_direction__switch_d	# case 'd':
	beq	$t0, '\n', input_direction__switch_newline	# case '\n':
	beq	$t0, 0,    input_direction__switch_null	# case '\0':
	beq	$t0, 4,    input_direction__switch_eot	# case '\004':
	j	input_direction__switch_default		# default:

input_direction__switch_w:
	li	$t0, NORTH			# direction = NORTH;
	j	input_direction__switch_post	# break;

input_direction__switch_a:
	li	$t0, WEST			# direction = WEST;
	j	input_direction__switch_post	# break;

input_direction__switch_s:
	li	$t0, SOUTH			# direction = SOUTH;
	j	input_direction__switch_post	# break;

input_direction__switch_d:
	li	$t0, EAST			# direction = EAST;
	j	input_direction__switch_post	# break;

input_direction__switch_newline:
	j	input_direction__do		# continue;

input_direction__switch_null:
input_direction__switch_eot:
	li	$v0, 17			# syscall 17: exit2
	li	$a0, 0
	syscall				# exit(0);

input_direction__switch_default:
	li	$v0, 4			# syscall 4: print_string
	la	$a0, input_direction__invalid_direction
	syscall				# printf("invalid direction: ");

	li	$v0, 11			# syscall 11: print_character
	move	$a0, $t0
	syscall				# printf("%c", direction);

	li	$v0, 11			# syscall 11: print_character
	li	$a0, '\n'
	syscall				# printf("\n");

	j	input_direction__do	# continue;

input_direction__switch_post:
	blt	$t0, 0, input_direction__bonk_branch	# if (0 <= direction ...
	bgt	$t0, 3, input_direction__bonk_branch	# ... && direction <= 3 ...

	lw	$t1, last_direction	#     last_direction
	sub	$t1, $t1, $t0		#     last_direction - direction
	abs	$t1, $t1		# abs(last_direction - direction)
	beq	$t1, 2, input_direction__bonk_branch	# ... && abs(last_direction - direction) != 2)

	sw	$t0, last_direction	# last_direction = direction;

	move	$v0, $t0
	jr	$ra			# return direction;

input_direction__bonk_branch:
	li	$v0, 4			# syscall 4: print_string
	la	$a0, input_direction__bonk
	syscall				# printf("bonk! cannot turn around 180 degrees\n");

input_direction__while:
	j	input_direction__do	# while (true);



########################################################################
# .TEXT <get_d_row>
	.text
get_d_row:

	# Args:
	#   - $a0: int direction
	# Returns:
	#   - $v0: int
	#
	# Frame:    None
	# Uses:     $v0, $a0
	# Clobbers: $v0
	#
	# Locals:   None
	#
	# Structure:
	#   get_d_row
	#   -> get_d_row__south:
	#   -> get_d_row__north:
	#   -> get_d_row__else:

	# Code:
	beq	$a0, SOUTH, get_d_row__south	# if (direction == SOUTH)
	beq	$a0, NORTH, get_d_row__north	# else if (direction == NORTH)
	j	get_d_row__else			# else

get_d_row__south:
	li	$v0, 1
	jr	$ra				# return 1;

get_d_row__north:
	li	$v0, -1
	jr	$ra				# return -1;

get_d_row__else:
	li	$v0, 0
	jr	$ra				# return 0;



########################################################################
# .TEXT <get_d_col>
	.text
get_d_col:

	# Args:
	#   - $a0: int direction
	# Returns:
	#   - $v0: int
	#
	# Frame:    None
	# Uses:     $v0, $a0
	# Clobbers: $v0
	#
	# Locals:   None
	#
	# Structure:
	#   get_d_col
	#   -> get_d_col__east:
	#   -> get_d_col__west:
	#   -> get_d_col__else:

	# Code:
	beq	$a0, EAST, get_d_col__east	# if (direction == EAST)
	beq	$a0, WEST, get_d_col__west	# else if (direction == WEST)
	j	get_d_col__else			# else

get_d_col__east:
	li	$v0, 1
	jr	$ra				# return 1;

get_d_col__west:
	li	$v0, -1
	jr	$ra				# return -1;

get_d_col__else:
	li	$v0, 0
	jr	$ra				# return 0;



########################################################################
# .TEXT <seed_rng>
	.text
seed_rng:

	# Args:
	#   - $a0: unsigned int seed
	# Returns:  void
	#
	# Frame:    None
	# Uses:     $a0
	# Clobbers: None
	#
	# Locals:   None
	#
	# Structure:
	#   seed_rng
	#   -> body

	# Code:
	sw	$a0, rand_seed		# rand_seed = seed;

	jr	$ra			# return;



########################################################################
# .TEXT <rand_value>
	.text
rand_value:

	# Args:
	#   - $a0: unsigned int n
	# Returns:
	#   - $v0: unsigned int
	#
	# Frame:    None
	# Uses:     $v0, $a0, $t0, $t1
	# Clobbers: $v0, $t0, $t1
	#
	# Locals:
	#   - `unsigned int rand_seed` cached in $t0
	#
	# Structure:
	#   rand_value
	#   -> body

	# Code:
	lw	$t0, rand_seed		#  rand_seed

	li	$t1, 1103515245
	mul	$t0, $t0, $t1		#  rand_seed * 1103515245

	addiu	$t0, $t0, 12345		#  rand_seed * 1103515245 + 12345

	li	$t1, 0x7FFFFFFF
	and	$t0, $t0, $t1		# (rand_seed * 1103515245 + 12345) & 0x7FFFFFFF

	sw	$t0, rand_seed		# rand_seed = (rand_seed * 1103515245 + 12345) & 0x7FFFFFFF;

	rem	$v0, $t0, $a0
	jr	$ra			# return rand_seed % n;

