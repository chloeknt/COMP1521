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
    for (int i = 1; i < argc; i++) {
        // Read a file 
        struct stat s;
        if (stat(argv[i], &s) != 0) {
            perror(argv[i]);
            return -1;
        }
        
        // Depending on the result, print the permissions
        printf( (S_ISDIR(s.st_mode)) ? "d" : "-");
        printf( (s.st_mode & S_IRUSR) ? "r" : "-");
        printf( (s.st_mode & S_IWUSR) ? "w" : "-");
        printf( (s.st_mode & S_IXUSR) ? "x" : "-");
        printf( (s.st_mode & S_IRGRP) ? "r" : "-");
        printf( (s.st_mode & S_IWGRP) ? "w" : "-");
        printf( (s.st_mode & S_IXGRP) ? "x" : "-");
        printf( (s.st_mode & S_IROTH) ? "r" : "-");
        printf( (s.st_mode & S_IWOTH) ? "w" : "-");
        printf( (s.st_mode & S_IXOTH) ? "x" : "-");
        printf(" %s", argv[i]);
        printf("\n");
    }

    return 0;
}

