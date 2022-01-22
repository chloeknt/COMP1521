#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // Check for correct arguments
    if (argc != 4) {
        printf("Wrong number of arguments\n");
        return -1;
    } else if (atoi(argv[2]) > atoi(argv[3])) {
        printf("Wrong order of integers\n");
        return -1;
    }
    
    // Open and write to a file 
    FILE *output_stream = fopen(argv[1], "w");
    if (output_stream == NULL) {
        perror(argv[1]);
        return 1;
    }

    // Integers loop
    int i = atoi(argv[2]);
    while (i <= atoi(argv[3])) {   
        fprintf(output_stream, "%d\n", i);
        i++;
    }

    // fclose will flush data to file
    fclose(output_stream);

    return 0;
}
