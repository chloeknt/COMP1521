// COMP1521 21T2 ... final exam, question 5

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 2048

void
print_utf8_count (FILE *file)
{
	unsigned long amount_1_byte = 0;
	unsigned long amount_2_byte = 0;
	unsigned long amount_3_byte = 0;
	unsigned long amount_4_byte = 0;

    char line[MAX];
	while (fgets(line, MAX, file) != NULL) {
	    int i = 0;
	    int count = 1;
	    while (line[i]) {
	        if ((line[i] & 0xc0) == 0x80) {
	            count++;
	        } else if ((line[i] & 0xc0) != 0x80) {
	            if (count == 1) {
	                amount_1_byte++;
	            } else if (count == 2) {
	            	amount_2_byte++;
	            } else if (count == 3) {
	                amount_3_byte++;
	            } else if (count == 4){
	                amount_4_byte++;
	            }
	            count = 1;
	        }
	        i++;
	    }	        
	}

	printf("1-byte UTF-8 characters: %lu\n", amount_1_byte);
	printf("2-byte UTF-8 characters: %lu\n", amount_2_byte);
	printf("3-byte UTF-8 characters: %lu\n", amount_3_byte);
	printf("4-byte UTF-8 characters: %lu\n", amount_4_byte);
}
