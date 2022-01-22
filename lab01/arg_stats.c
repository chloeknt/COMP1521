// Given one or more command line arguments, it prints the minimum and 
// maximum values, the sum and product of all the values, and the 
// mean of all values.
// Written by Chloe Toh 
// z5362296 
// 05/06/2021

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc == 1) {
	    printf("Usage: ./arg_stats NUMBER [NUMBER ...]\n");
	} else {
        //Minimum 
        int i = 2;	
        int min_value = atoi(argv[1]);
        while (i < argc) {
            if (atoi(argv[i]) < min_value) {
                min_value = atoi(argv[i]);
            }
            i++;
        }        
        //Maximum
        int j = 2;	
        int max_value = atoi(argv[1]);
        while (j < argc) {
            if (atoi(argv[j]) > max_value) {
                max_value = atoi(argv[j]);
            }
            j++;
        }   
        //Sum
        //Prod
        int sum = 0;
        int prod = 1;
        int counter = 1;
        while (argv[counter] != NULL) {
            sum = sum + atoi(argv[counter]);
            prod = prod * atoi(argv[counter]);
            counter++;
        }  
        //Mean
        int mean = sum / (counter-1);
        printf("MIN:  %d\n", min_value);
        printf("MAX:  %d\n", max_value);
        printf("SUM:  %d\n", sum);
        printf("PROD: %d\n", prod);
        printf("MEAN: %d\n", mean);
	}
	return 0;
}

