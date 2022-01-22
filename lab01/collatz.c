// Takes a single positive integer as a command-line argument and prints
// the collatz chain for that number. 
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <stdlib.h>

void collatz(int number); 

int main(int argc, char **argv)
{
	int number = 0;
	    number = strtol(argv[1], NULL, 10);
	    printf("%d\n",  number);
	    collatz(number);
	return EXIT_SUCCESS;
}

void collatz(int number) {
    if (number != 1) {
        //Even
        if (number % 2 == 0) {
            number = number / 2;
        //Odd
        } else {
            number = (3 * number) + 1;
        }
        printf("%d\n", number);     
        collatz(number);
    } 
}
