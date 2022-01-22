// Given a line of input containing a natural number the corresponding 
// fibonacci number is printed.It should never calculate the same fibonacci
// number twice. 
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <stdlib.h>

#define SERIES_MAX 46

int fibonacci(int num, int already_computed[SERIES_MAX + 1]);

int main(void) {    
    int already_computed[SERIES_MAX + 1] = {0,1};
    int number;
    while (scanf("%d", &number) != EOF) {   
        printf("%d\n", fibonacci(number, already_computed));
    }
    return EXIT_SUCCESS; 
}

int fibonacci(int num, int already_computed[SERIES_MAX + 1]) {    
    if(num == 0 || num == 1) {  
        already_computed[num] = num;
        return num;
    } else if (already_computed[num] == 0) {
        already_computed[num] = fibonacci(num - 1, already_computed) + 
        fibonacci(num - 2, already_computed);
        return already_computed[num];
    } else {
        return already_computed[num];
    }

}
