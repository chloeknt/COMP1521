// COMP1521 21T2 ... final exam, question 7

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 2048

const mode_t NEW_DIR_MODE = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

void
cp_directory (char *dir_from, char *dir_to)
{
	DIR *dir = opendir(dir_from);
	mkdir(dir_to, NEW_DIR_MODE);
    //DIR *dest_dir = opendir(dir_to);
	struct dirent *entry;
	char *tmpstr;
	while ((entry = readdir(dir)) != NULL) {
	    tmpstr = entry->d_name;
        struct stat s;
        stat(tmpstr, &s);
        if (S_ISDIR(s.st_mode)) {
            char *new_strpath = tmpstr;
            cp_directory(tmpstr, new_strpath);
        } else {
            FILE *input = fopen(tmpstr, "r");
            FILE *output = fopen("tmp", "w");
            int c;
            while ((c = fgetc(input)) != EOF) {
                fputc(c, output);
            }
            
            fclose(input);
            fclose(output);
	    }
	}
}
