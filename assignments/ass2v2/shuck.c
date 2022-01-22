////////////////////////////////////////////////////////////////////////
// COMP1521 21t2 -- Assignment 2 -- shuck, A Simple Shell
// <https://www.cse.unsw.edu.au/~cs1521/21T2/assignments/ass2/index.html>
//
// Written by Chloe Toh (z5362296) on 05/08/21.
//
// 2021-07-12    v1.0    Team COMP1521 <cs1521@cse.unsw.edu.au>
// 2021-07-21    v1.1    Team COMP1521 <cs1521@cse.unsw.edu.au>
//     * Adjust qualifiers and attributes in provided code,
//       to make `dcc -Werror' happy.
//

#include <sys/types.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <glob.h>
#include <sys/stat.h>

#define PATH_MAX 2048
//
// Interactive prompt:
//     The default prompt displayed in `interactive' mode --- when both
//     standard input and standard output are connected to a TTY device.
//
static const char *const INTERACTIVE_PROMPT = "shuck& ";

//
// Default path:
//     If no `$PATH' variable is set in Shuck's environment, we fall
//     back to these directories as the `$PATH'.
//
static const char *const DEFAULT_PATH = "/bin:/usr/bin";

//
// Default history shown:
//     The number of history items shown by default; overridden by the
//     first argument to the `history' builtin command.
//     Remove the `unused' marker once you have implemented history.
//
static const int DEFAULT_HISTORY_SHOWN __attribute__((unused)) = 10;

//
// Input line length:
//     The length of the longest line of input we can read.
//
static const size_t MAX_LINE_CHARS = 1024;

//
// Special characters:
//     Characters that `tokenize' will return as words by themselves.
//
static const char *const SPECIAL_CHARS = "!><|";

//
// Word separators:
//     Characters that `tokenize' will use to delimit words.
//
static const char *const WORD_SEPARATORS = " \t\r\n";


static void execute_command(char **words, char **path, char **environment);
static void do_exit(char **words);
static int is_executable(char *pathname);
static char **tokenize(char *s, char *separators, char *special_chars);
static void free_tokens(char **tokens);
static bool is_slash(char **words);
static void cd(char **words);
static void pwd(char **words);
static bool is_command(char **words, char **path, char *pathname);
static bool redirected(char **found_words, char **words, char *pathname, int last);
static void do_command(char **words, char *pathname);
static void add_history(char **words);
static void print_history(int n);
static void execute_history(int n, char **path);
static char **glob_check(char **words);
static void input_file(char **words, char *pathname, int file);
static void output_file(char **words, char *pathname, int file, int mode);
static void inout_file(char **words, char *pathname, int file1, int file2, int mode);

int main (void)
{
    // Ensure `stdout' is line-buffered for autotesting.
    setlinebuf(stdout);

    // Environment variables are pointed to by `environ', an array of
    // strings terminated by a NULL value -- something like:
    //     { "VAR1=value", "VAR2=value", NULL }
    extern char **environ;

    // Grab the `PATH' environment variable for our path.
    // If it isn't set, use the default path defined above.
    char *pathp;
    if ((pathp = getenv("PATH")) == NULL) {
        pathp = (char *) DEFAULT_PATH;
    }
    char **path = tokenize(pathp, ":", "");

    // Should this shell be interactive?
    bool interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);

    // Main loop: print prompt, read line, execute command
    while (1) {
        // If `stdout' is a terminal (i.e., we're an interactive shell),
        // print a prompt before reading a line of input.
        if (interactive) {
            fputs(INTERACTIVE_PROMPT, stdout);
            fflush(stdout);
        }

        char line[MAX_LINE_CHARS];
        if (fgets(line, MAX_LINE_CHARS, stdin) == NULL)
            break;

        // Tokenise and execute the input line.
        char **command_words =
            tokenize(line, (char *) WORD_SEPARATORS, (char *) SPECIAL_CHARS);
        execute_command(command_words, path, environ);
        free_tokens(command_words);
    }

    free_tokens(path);
    return 0;
}


//
// Execute a command, and wait until it finishes.
//
//  * `words': a NULL-terminated array of words from the input command line
//  * `path': a NULL-terminated array of directories to search in;
//  * `environment': a NULL-terminated array of environment variables.
//
static void execute_command(char **words, char **path, char **environment)
{
    assert(words != NULL);
    assert(path != NULL);
    assert(environment != NULL);

    char *program = words[0];

    if (program == NULL) {
        // nothing to do
        return;
    }

    if (strcmp(program, "exit") == 0) {
        do_exit(words);
        // `do_exit' will only return if there was an error.
        return;
    }
    
    // Check for globbing
    char **found_words = malloc(1 * sizeof(*found_words));
    found_words = glob_check(words);
    
    char pathname[PATH_MAX];
    bool command = false;
    
    // Check for slash commands
    bool slash = is_slash(words);
    if (is_slash(words) && is_executable(words[0])) {
        command = true;
        strcpy(pathname, words[0]);
    } else if (!is_slash(words)) {
        command = is_command(words, path, pathname);
    }
    
    // Get last 'word'
    int last = 0;
    while (words[last + 1] != NULL) {
        last++;
    }
    
    // I/O redirection symbols
    if (redirected(found_words, words, pathname, last)) {
        return;
    }

    // Cd
    if (strcmp(words[0], "cd") == 0) {
        cd(words);
        return;
    }
    // Pwd
    if (strcmp(words[0], "pwd") == 0) {
        pwd(words);
        return;
    }
    
    // History commands
    if (strcmp(words[0], "history") == 0) {
        if (words[1] == NULL) {
            print_history(DEFAULT_HISTORY_SHOWN);
        } else if (words[2] != NULL) {
            fprintf(stderr, "history: too many arguments\n");
            return;
        } else if (atoi(words[1]) == 0) {
            fprintf(stderr, "history: nonnumber: numeric argument required\n");
            return;
        } else {
            print_history(atoi(words[1]));
        }
        add_history(words);
        return;
    } else if (strcmp(words[0], "!") == 0) {
        if (words[1] == NULL) {
            execute_history(-1, path);
        } else {
            execute_history(atoi(words[1]), path);
        }
        return;
        // Not any of the above and unexecutable file
    } else if (!command) {
        if (strcmp(words[0], "<") == 0) {
            fprintf(stderr, "%s: command not found\n", words[2]);
        } else {
            fprintf(stderr, "%s: command not found\n", words[0]);
        }
        return;
        // Slash commands 
    } else if (slash) {
        do_command(found_words, words[0]);
        // Non slash commands
    } else {
        do_command(found_words, pathname);
    }
    // Add to history
    add_history(words);
    return;
    
    if (strrchr(program, '/') == NULL) {
        fprintf(stderr, "--- UNIMPLEMENTED: searching for a program to run\n");
    }

    if (is_executable(program)) {
        fprintf(stderr, "--- UNIMPLEMENTED: running a program\n");
    } else {
        fprintf(stderr, "--- UNIMPLEMENTED: error when we can't run anything\n");
    }
}

// Function checks if there is a slash in command 
static bool is_slash(char **words) {
    char command_line[strlen(words[0]) + 1];
    strcpy(command_line, words[0]);
    int c = 0;
    while (command_line[c] != '\0') {
        if (command_line[c] == '/') {
            return true;
        }
        c++;
    }
    return false;
}

// Function executes cd command
static void cd(char **words) {
    // If there are no extra words, go to HOME 
    if (words[1] == NULL) {
        char *value = getenv("HOME");
        if (chdir(value) != 0) {
            perror("chdir");
            return;
        }
    } else {
        // If there are words, go there
        if (chdir(words[1]) != 0) {
            fprintf(stderr, "cd: %s: No such file or directory\n", words[1]);
            return;
        }
    }
    // Add command to history
    add_history(words);
    return;
}

// Function executes pwd command
static void pwd(char **words) {
    char pathname[PATH_MAX];
    // Getting the current working directory 
    if (getcwd(pathname, sizeof pathname) == NULL) {
        perror("getcwd");
        return;
    }
    printf("current directory is '%s'\n", pathname);
        
    // Add to history 
    add_history(words);
    return;
}

// Function determines if the command is executable 
static bool is_command(char **words, char **path, char *pathname) {
    int i = 0;
    // Loop through all paths and check if executable
    while (path[i] != NULL) {
        strcpy(pathname, path[i]);
        strcat(pathname, "/");
        if (strcmp(words[0], "<") == 0) {
            strcat(pathname, words[2]);
        } else {
            strcat(pathname, words[0]);
        }
        if (is_executable(pathname)) {
            return true;
        }
        i++;
    }
    return false;
}

// Function returns if any symbols indicated redirection and executes if true or error
static bool redirected(char **found_words, char **words, char *pathname, int last) {
    // Check if < or > appears anywhere else
    for (int i = 0; found_words[i] != NULL; i++) {
        // Input symbol
        if (i != 0 && strcmp(found_words[i], "<") == 0) {
            fprintf(stderr, "Redirection symbols placed incorrectly\n");
            return true;
            // Output symbol
        } else if (i != last - 1 && i != last - 2 && strcmp(found_words[i], ">") == 0) {
            fprintf(stderr, "Redirection symbols placed incorrectly\n");
            return true;
        }
    }
    // Input and output
    if (strcmp(found_words[0], "<") == 0 && strcmp(found_words[last - 2], ">") == 0 && 
    strcmp(found_words[last - 1], ">") == 0) {
        inout_file(found_words, pathname, 1, last, 3);
        return true;
    } else if (strcmp(found_words[0], "<") == 0 && strcmp(found_words[last - 1], ">") == 0) {       
        inout_file(found_words, pathname, 1, last, 2);
        return true;
        // Only input
    } else if (found_words[1] != NULL && strcmp(found_words[0], "<") == 0) {
        input_file(found_words, pathname, 1);
        add_history(words);
        return true;
        //Only output
    } else if (last > 2 && strcmp(found_words[last - 2], ">") == 0 && 
    strcmp(found_words[last - 1], ">") == 0) {
        output_file(found_words, pathname, last, 3);
        add_history(words);
        return true;
    } else if (last > 1 && strcmp(found_words[last - 1], ">") == 0) {
        output_file(found_words, pathname, last, 2);
        add_history(words);
        return true;
    }
    return false;
}

// Function executes a particular command
static void do_command(char **words, char *pathname) {
    pid_t pid;
    extern char **environ;
    // Spawn processes with specific command
    if (posix_spawn(&pid, pathname, NULL, NULL, words, environ) != 0) {
        perror(words[0]);
        return;
    }

    // Wait for spawned processes to finish
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(exit_status)) {
        exit_status = WEXITSTATUS(exit_status);
    }
    
    printf("%s exit status = %d\n", pathname, exit_status);       
    
    return;
}

// Function records history
static void add_history(char **words) {
    // Get history file 
    char *home = getenv("HOME");
    char history[strlen(home) + 16];
    strcpy(history, home);
    strcat(history, "/.shuck_history");
    
    // Open and append to a file 
    FILE *stream = fopen(history, "a+");
    if (stream == NULL) {
        perror(history);
        return;
    }

    rewind(stream);
    char line[MAX_LINE_CHARS];
    // Go to the end of the file 
    while (fgets(line, MAX_LINE_CHARS, stream) != NULL);
    int i = 0;
    // Add command to file
    while (words[i + 1] != NULL) {
        fprintf(stream, "%s ", words[i]);
        i++;
    }
    fprintf(stream, "%s\n", words[i]);
    
    fclose(stream);

    return;
}

// Function prints last n commands
static void print_history(int n) {
    // Get history
    char *home = getenv("HOME");
    char history[strlen(home) + 16];
    strcpy(history, home);
    strcat(history, "/.shuck_history");
    
    // Open and read a file 
    FILE *stream = fopen(history, "r");
    if (stream == NULL) {
        perror(history);
        return;
    }
    
    // Go to the end of the file, count lines
    char last_line[MAX_LINE_CHARS];
    int pos = 0;
    while (fgets(last_line, MAX_LINE_CHARS, stream) != NULL) {
        pos++;
    }
    int new_pos = pos - n;
    // Print last n commands 
    rewind(stream);
    char line[MAX_LINE_CHARS];
    int line_pos = 0;
    while (fgets(line, MAX_LINE_CHARS, stream) != NULL) {
        if (line_pos >= new_pos) {
            printf("%d: %s", line_pos, line);
        }
        line_pos++;
    }
    
    fclose(stream);
    
    return;
}

// Function finds relevant history command and executes it 
static void execute_history(int n, char **path) {
    // Get history
    char *home = getenv("HOME");
    char history[strlen(home) + 16];
    strcpy(history, home);
    strcat(history, "/.shuck_history");
    
    // Open and read a file 
    FILE *stream = fopen(history, "r");
    if (stream == NULL) {
        perror(history);
        return;
    }
    
    extern char **environ;
    
    // If no number is specified, get last command
    if (n == -1) {
        char last_line[MAX_LINE_CHARS];
        // Find the last line
        while (fgets(last_line, MAX_LINE_CHARS, stream) != NULL);
        char command_line[MAX_LINE_CHARS];
        int i = 0;
        // Loop and discard newline
        while (last_line[i] != '\0') {
            command_line[i] = last_line[i];
            i++;
        }
        command_line[i - 1] = '\0';
        char **words = tokenize(command_line, " ", "");
        printf("%s\n", command_line);
        // Send this command line to function to execute
        execute_command(words, path, environ);
    } else {
        char line[MAX_LINE_CHARS];
        int line_pos = 0;
        // Loop until specific line
        while (fgets(line, MAX_LINE_CHARS, stream) != NULL) {
            if (line_pos == n) {
                char command_line[MAX_LINE_CHARS];
                int i = 0;
                // Loop and discard newline
                while (line[i] != '\0') {
                    command_line[i] = line[i];
                    i++;
                }
                command_line[i - 1] = '\0';
                char **command_words = tokenize(command_line, " ", "");
                printf("%s\n", command_line);
                // Send this command line to function to execute
                execute_command(command_words, path, environ);
                break;
            }
            line_pos++;
        }
    }
    fclose(stream);
    
    return;
}

// Function checks if any globbing symbols exists, and if so, returns a 
// new string array with all found words 
static char **glob_check(char **words) {
    // Create a new array of strings 
    char **found_words = malloc(1 * sizeof(*found_words));
    // Add a copy of the command 
    found_words[0] = malloc((strlen(words[0]) + 1) * sizeof(char));
    char *command = strndup(words[0], strlen(words[0]));
    found_words[0] = command;

    int i = 1;
    int pos = 1;
    int num_matches = 0;
    // Determine number of matches in total
    // Loop through all the words other than the command
    while (words[i] != NULL) { 
        glob_t matches; 
        int result = glob(words[i], GLOB_NOCHECK|GLOB_TILDE, NULL, &matches);
        if (result != 0) { 
            perror("glob");
            exit(1);
        } else {
            // Update the number of matches found 
            num_matches += (int)matches.gl_pathc; 
            // Allocate more memory for the matches found
            found_words = realloc(found_words, ((num_matches + 1) * 
            sizeof(*found_words)));
            // Loop and insert copy of the found word into the array
            for (int c = 0; c < matches.gl_pathc; c++) {
                found_words[pos] = malloc((strlen(matches.gl_pathv[c]) + 1) 
                * sizeof(char));
                char *found = strndup(matches.gl_pathv[c], 
                strlen(matches.gl_pathv[c])); 
                found_words[pos] = found;
                pos++;
            }
        }
        i++;
    }
    // Allocate memory for and insert NULL at the end
    found_words = realloc(found_words, ((num_matches + 2) * sizeof(*found_words)));
    found_words[num_matches + 1] = NULL;
    return found_words;
}


// Fuction executes input redirection
static void input_file(char **words, char *pathname, int file) {
    // If the command is built-in, return error message
    if ((strcmp(words[2], "history")) == 0 || (strcmp(words[2], "cd")) == 0
    || (strcmp(words[2], "pwd")) == 0) {
        fprintf(stderr, "%s: I/O redirection not permitted for builtin commands\n", words[2]);
        return;
    }

    // Get the current working directory to find file 
    char filepath[PATH_MAX];        
    if (getcwd(filepath, sizeof filepath) == NULL) {
        perror("getcwd");
        return;
    }
    strcat(filepath, "/");
    strcat(filepath, words[file]);

    pid_t pid;
    extern char **environ;

    // Create list of actions
    posix_spawn_file_actions_t actions;
    if (posix_spawn_file_actions_init(&actions) != 0) {
        perror("posix_spawn_file_actions_init");
        return;
    }

    // Add opening the file for reading to actions
    if (posix_spawn_file_actions_addopen(&actions, 3, filepath, O_RDONLY, 0644) != 0) {
        perror("posix_spawn_file_actions_addopen");
        return;
    }

    // Add the redirection of input to actions
    if (posix_spawn_file_actions_adddup2(&actions, 3, STDIN_FILENO) != 0) {
        perror("posix_spawn_file_actions_adddup2");
        return;
    }
    
    // Add the closure of file 
    if (posix_spawn_file_actions_addclose(&actions, 3) != 0) {
        perror("posix_spawn_file_actions_close");
        return;
    }
    
    // Count the words needed 
    int count = 1;
    while (words[count] != NULL) {
        count++;
    }
    
    // Place into one string 
    char *command_argv[count + 1];
    command_argv[0] = words[2];
    int i = 1;
    int pos = 3;
    while (words[pos] != NULL) {
        command_argv[i] = words[pos];
        i++;
        pos++;
    }
    command_argv[i] = NULL;
    
    // Spawn new process to run command
    if (posix_spawn(&pid, pathname, &actions, NULL, command_argv, environ) != 0) {
        perror(pathname);
        return;
    }

    // Wait for spawned process to finish
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
        perror("waitpid");
        return;
    }
    
    if (WIFEXITED(exit_status)) {
        exit_status = WEXITSTATUS(exit_status);
    }
    
    printf("%s exit status = %d\n", pathname, exit_status);
    
    // Destroy actions
    posix_spawn_file_actions_destroy(&actions);

    return;
} 

// Function executes output redirection 
static void output_file(char **words, char *pathname, int file, int mode) {
    // If the command is built-in, return error message
    if ((strcmp(words[0], "history")) == 0 || (strcmp(words[0], "cd")) == 0
    || (strcmp(words[0], "pwd")) == 0) {
        fprintf(stderr, "%s: I/O redirection not permitted for builtin commands\n", words[0]);
        return;
    } 
    
    // Get current working directory to find file 
    char filepath[PATH_MAX];        
    if (getcwd(filepath, sizeof filepath) == NULL) {
        perror("getcwd");
        return;
    }
    strcat(filepath, "/");
    strcat(filepath, words[file]);

    pid_t pid;
    extern char **environ;

    // Create list of actions
    posix_spawn_file_actions_t actions;
    if (posix_spawn_file_actions_init(&actions) != 0) {
        perror("posix_spawn_file_actions_init");
        return;
    }
    
    // If >, add opening of file for over-writing to actions
    if (mode == 2) {
        if (posix_spawn_file_actions_addopen(&actions, 3, filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644) != 0) {
            perror("posix_spawn_file_actions_addopen");
            return;
        }
        // If >>, add opening of file for appending to actions
    } else {
        if (posix_spawn_file_actions_addopen(&actions, 3, filepath, O_WRONLY | O_APPEND, 0644) != 0) {
            perror("posix_spawn_file_actions_addopen");
            return; 
        }
    }

    // Add the redirection of output to actions 
    if (posix_spawn_file_actions_adddup2(&actions, 3, STDOUT_FILENO) != 0) {
        perror("posix_spawn_file_actions_adddup2");
        return;
    }

    // Add closure of file to actions
    if (posix_spawn_file_actions_addclose(&actions, 3) != 0) {
        perror("posix_spawn_file_actions_addclose");
        return;
    }
    
    // Count the words needed 
    int count = 0;
    while (words[count + mode] != NULL) {
        count++;
    }
        
    // Place into one string 
    char *command_argv[count + 1];
    command_argv[0] = words[0];
    int i = 1;
    while (words[i + mode] != NULL) {
        command_argv[i] = words[i];
        i++;
    }
    command_argv[i] = NULL;
    
    // Spawn new process to run command
    if (posix_spawn(&pid, pathname, &actions, NULL, command_argv, environ) != 0) {
        perror(pathname);
        return;
    }

    // Wait for spawned processes to finish 
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
        perror("waitpid");
        return;
    }
    
    if (WIFEXITED(exit_status)) {
        exit_status = WEXITSTATUS(exit_status);
    }
    
    printf("%s exit status = %d\n", pathname, exit_status);

    // Destroy actions
    posix_spawn_file_actions_destroy(&actions);

    return;
}

// Function executes both input and output redirection
static void inout_file(char **words, char *pathname, int file1, int file2, int mode) {
    // If the command is built-in, return error message 
    if ((strcmp(words[2], "history")) == 0 || (strcmp(words[2], "cd")) == 0
    || (strcmp(words[2], "pwd")) == 0) {
        fprintf(stderr, "%s: I/O redirection not permitted for builtin commands\n", words[2]);
        return;
    }

    // Get current working directory to find file one
    char filepath1[PATH_MAX];        
    if (getcwd(filepath1, sizeof filepath1) == NULL) {
        perror("getcwd");
        return;
    }
    strcat(filepath1, "/");
    strcat(filepath1, words[file1]);
    
    // Get current working directory to find file two
    char filepath2[PATH_MAX];        
    if (getcwd(filepath2, sizeof filepath2) == NULL) {
        perror("getcwd");
        return;
    }
    strcat(filepath2, "/");
    strcat(filepath2, words[file2]);

    pid_t pid;
    extern char **environ;

    // Create list of actions
    posix_spawn_file_actions_t actions;
    if (posix_spawn_file_actions_init(&actions) != 0) {
        perror("posix_spawn_file_actions_init");
        return;
    }

    // Add opening file one for reading to actions
    if (posix_spawn_file_actions_addopen(&actions, 3, filepath1, O_RDONLY, 0644) != 0) {
        perror("posix_spawn_file_actions_addopen");
        return;
    }
    
    // If >, add opening file two for over-writing to actions
    if (mode == 2) {
        if (posix_spawn_file_actions_addopen(&actions, 4, filepath2, O_WRONLY | O_CREAT | O_TRUNC, 0644) != 0) {
            perror("posix_spawn_file_actions_addopen");
            return;
        }
        // If >>, add opening file two for appending to actions
    } else {
        if (posix_spawn_file_actions_addopen(&actions, 4, filepath2, O_WRONLY | O_APPEND, 0644) != 0) {
            perror("posix_spawn_file_actions_addopen");
            return;
        }
    }
      
    // Add the redirection of input to actions
    if (posix_spawn_file_actions_adddup2(&actions, 3, STDIN_FILENO) != 0) {
        perror("posix_spawn_file_actions_adddup2");
        return;
    }
    
    // Add the redirection of output to actions
    if (posix_spawn_file_actions_adddup2(&actions, 4, STDOUT_FILENO) != 0) {
        perror("posix_spawn_file_actions_adddup2");
        return;
    }
    
    // Add closure of file one to actions
    if (posix_spawn_file_actions_addclose(&actions, 3) != 0) {
        perror("posix_spawn_file_actions_close");
        return;
    }
    
    // Add closure of file two to actions
    if (posix_spawn_file_actions_addclose(&actions, 4) != 0) {
        perror("posix_spawn_file_actions_close");
        return;
    }
    
    // Count the words needed 
    int count = 1;
    while (strcmp(words[count], ">") != 0) {
        count++;
    }
    
    // Place into one string 
    char *command_argv[count + 1];
    command_argv[0] = words[2];
    int i = 1;
    int pos = 3;
    while (strcmp(words[pos], ">") != 0) {
        command_argv[i] = words[pos];
        i++;
        pos++;
    }
    command_argv[i] = NULL;

    // Spawn new process to run command
    if (posix_spawn(&pid, pathname, &actions, NULL, command_argv, environ) != 0) {
        perror(pathname);
        return;
    }

    // Wait for processes to finish
    int exit_status;
    if (waitpid(pid, &exit_status, 0) == -1) {
        perror("waitpid");
        return;
    }
    
    if (WIFEXITED(exit_status)) {
        exit_status = WEXITSTATUS(exit_status);
    }
    
    printf("%s exit status = %d\n", pathname, exit_status);
    
    // Destroy actions
    posix_spawn_file_actions_destroy(&actions);

    return;
} 

//
// Implement the `exit' shell built-in, which exits the shell.
//
// Synopsis: exit [exit-status]
// Examples:
//     % exit
//     % exit 1
//
static void do_exit(char **words)
{
    assert(words != NULL);
    assert(strcmp(words[0], "exit") == 0);

    int exit_status = 0;

    if (words[1] != NULL && words[2] != NULL) {
        // { "exit", "word", "word", ... }
        fprintf(stderr, "exit: too many arguments\n");

    } else if (words[1] != NULL) {
        // { "exit", something, NULL }
        char *endptr;
        exit_status = (int) strtol(words[1], &endptr, 10);
        if (*endptr != '\0') {
            fprintf(stderr, "exit: %s: numeric argument required\n", words[1]);
        }
    }

    exit(exit_status);
}


//
// Check whether this process can execute a file.  This function will be
// useful while searching through the list of directories in the path to
// find an executable file.
//
static int is_executable(char *pathname)
{
    struct stat s;
    return
        // does the file exist?
        stat(pathname, &s) == 0 &&
        // is the file a regular file?
        S_ISREG(s.st_mode) &&
        // can we execute it?
        faccessat(AT_FDCWD, pathname, X_OK, AT_EACCESS) == 0;
}


//
// Split a string 's' into pieces by any one of a set of separators.
//
// Returns an array of strings, with the last element being `NULL'.
// The array itself, and the strings, are allocated with `malloc(3)';
// the provided `free_token' function can deallocate this.
//
static char **tokenize(char *s, char *separators, char *special_chars)
{
    size_t n_tokens = 0;

    // Allocate space for tokens.  We don't know how many tokens there
    // are yet --- pessimistically assume that every single character
    // will turn into a token.  (We fix this later.)
    char **tokens = calloc((strlen(s) + 1), sizeof *tokens);
    assert(tokens != NULL);

    while (*s != '\0') {
        // We are pointing at zero or more of any of the separators.
        // Skip all leading instances of the separators.
        s += strspn(s, separators);

        // Trailing separators after the last token mean that, at this
        // point, we are looking at the end of the string, so:
        if (*s == '\0') {
            break;
        }

        // Now, `s' points at one or more characters we want to keep.
        // The number of non-separator characters is the token length.
        size_t length = strcspn(s, separators);
        size_t length_without_specials = strcspn(s, special_chars);
        if (length_without_specials == 0) {
            length_without_specials = 1;
        }
        if (length_without_specials < length) {
            length = length_without_specials;
        }

        // Allocate a copy of the token.
        char *token = strndup(s, length);
        assert(token != NULL);
        s += length;

        // Add this token.
        tokens[n_tokens] = token;
        n_tokens++;
    }

    // Add the final `NULL'.
    tokens[n_tokens] = NULL;

    // Finally, shrink our array back down to the correct size.
    tokens = realloc(tokens, (n_tokens + 1) * sizeof *tokens);
    assert(tokens != NULL);

    return tokens;
}


//
// Free an array of strings as returned by `tokenize'.
//
static void free_tokens(char **tokens)
{
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}
