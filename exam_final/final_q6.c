// COMP1521 21T2 ... final exam, question 6

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX 2048

void
convert_hexdump_to_bytes (FILE *hexdump_input, FILE *byte_output)
{
    char address[9];
    char line[MAX];
    int i = 0;
    while (fgets(line, MAX, hexdump_input) != NULL) {
        fscanf(hexdump_input, "%s", address);
        printf("%d\n", atoi(address));
	    i++;
	}
	return;
}
