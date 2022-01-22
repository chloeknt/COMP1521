#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // Check for correct arguments
    if (argc != 2) {
        printf("Wrong number of arguments\n");
        return 1;
    } 
    
    // Open and read
    FILE *input_stream = fopen(argv[1], "r");
    if (input_stream == NULL) {
        perror(argv[1]);
        return 1;
    }

    // Byte loop
    int c;
    int i = 0;
    while ((c = fgetc(input_stream)) != EOF) {
        if (c >= 128 && c <= 255) {
            printf("%s: byte %d is non-ASCII\n", argv[1], i);
            fclose(input_stream);
            return 0;
        } 
        i++;
    }
    
    printf("%s is all ASCII\n", argv[1]);

    fclose(input_stream);

    return 0;
}