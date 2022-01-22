#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
    // Check for correct arguments
    if (argc < 2) {
        printf("Wrong number of arguments\n");
        return -1;
    } 
    
    // Loop through files
    long total = 0;
    for (int i = 1; i < argc; i++) {
        // Read a file 
        struct stat s;
        if (stat(argv[i], &s) != 0) {
            perror(argv[i]);
            return -1;
        }
        printf("%s: %ld bytes\n", argv[i], (long)s.st_size);
        total += (long)s.st_size;
    }
    
    // Print total
    printf("Total: %ld bytes\n", (long)total);

    return 0;
}
