#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define WELCOME "Welcome in the shell ENSEA. \nTo exit, type 'exit'.\n"
#define PROMPT  "enseash % "
#define BUFFER_SIZE 256
#define BYE "Bye bye ...\n"

int main(void) {
    char buffer[BUFFER_SIZE];		//User input buffer
    char *args[BUFFER_SIZE / 2 + 1];	//Arg array for execvp
    char *outfile;			//Output file for redirection
    ssize_t bytes_read;

    int last_exit = 0;			//Exit code of previous command
    int last_signal = -1;		//Signal number if killed
    int first_cmd = 1;			//Prevent status display at startup
    long elapsed_ms = 0;		//Execution time of previous command

    write(STDOUT_FILENO, WELCOME, strlen(WELCOME));	//Display welcome message

    while (1) {
	//Display prompt prefix (status + execution time)
	display_prompt(first_cmd, last_exit, last_signal, elapsed_ms);
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	
	//Read user input
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

	//End of file (Ctrl+D)
        if (bytes_read == 0) {
            write(STDOUT_FILENO, "\n", 1);
            write(STDOUT_FILENO, BYE, strlen(BYE));
            return 0;
        }

        buffer[bytes_read - 1] = '\0';
	
	//Exit command
        if (strcmp(buffer, "exit") == 0) {
            write(STDOUT_FILENO, BYE, strlen(BYE));
            return 0;
        }
	
	//Parse command and detect redirection
	int redirect = parse_command(buffer, args, &outfile);
	
	//Execute command, retrieve status and execution time
	execute_command(args, redirect, outfile, &last_exit, &last_signal, &elapsed_ms);

	//Now a status can be displayed
        first_cmd = 0;
    }
}
