#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    
    int num = 0;
    FILE *input = fopen(argv[2], "r");
    while (fgetc(input) != EOF) {
        num++;
    } 
    rewind(input);
    
    if ((num - n) <= 0) {
        FILE *empty = fopen(argv[3], "w");
        fclose(empty);
        fclose(input);
        return 0;
    }
    
    FILE *output = fopen(argv[3], "w");
    int i = 0;
    char c;
    while (i < (num - n)) {
        c = fgetc(input);
        fputc(c, output);
        i++;
    }
    
    fclose(input);
    fclose(output);
    
    return 0;
}
