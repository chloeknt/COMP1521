#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>


int main(int argc, char *argv[]) {  
    struct stat s;
    if (stat(argv[1], &s) != 0) {
        printf("0\n");
        return 0;
    }

    if (S_ISDIR(s.st_mode) != 0) {
        printf("1\n");
    } else {
        printf("0\n");
    }
    
    return 0;
}
