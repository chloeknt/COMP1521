// Given text on stdin, only print lines with an even number of characters
// to stdout.
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <string.h>

#define MAX 1024

int main(void) {
    char str[MAX];
    while (fgets(str, MAX, stdin) != NULL) {
        if (strlen(str) % 2 == 0) {
            fputs(str, stdout);
        } 
    }
	return 0;
}


