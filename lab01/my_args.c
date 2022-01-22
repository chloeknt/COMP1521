// Given 0 or more command line arguments, the command line arguments are 
// "pretty printed".
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>

#define MAX 1024

int main(int argc, char **argv) {
    printf("Program name: %s\n", argv[0]);
    if (argc == 1) {
        printf("There are no other arguments\n");
    } else if (argc > 1) {
        printf("There are %d arguments:\n", argc-1);
        int i = 1;
        while (i < argc) {          
            printf("\tArgument %d is \"%s\"\n", i, argv[i]);
            i++;
        }
    }
	return 0;
}
