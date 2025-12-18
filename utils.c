#include <unistd.h>	//write, fork, exec
#include <string.h>	//strcmp, strtok
#include <sys/wait.h>	//wait and status macros
#include <stdlib.h>	//exit
#include <time.h>	//clock_gettime
#include <fcntl.h>	//open (redirection)
#include "utils.h"

void write_number(int n){//recursive function  -> displays the digit one by one
    char c;
    if (n >= 10)
        write_number(n / 10);	//Recursive call -> print higher digits first
    c = '0' + (n % 10);		//Convert digit to ASCII
    write(STDOUT_FILENO, &c, 1);
}

// Display shell prompt with status (exit or signal) and execution time 
void display_prompt(int first_cmd, int last_exit, int last_signal, long elapsed_ms)
{
    if (first_cmd)	//display something only after the first command execution
        return;

    if (last_signal != -1) {
        write(STDOUT_FILENO, "[sign:", 6);
        write_number(last_signal);
    } else {
        write(STDOUT_FILENO, "[exit:", 6);
        write_number(last_exit);
    }

    write(STDOUT_FILENO, "|", 1);
    write_number(elapsed_ms);
    write(STDOUT_FILENO, "ms] ", 4);
}

// parse the user input + fill the argument array for execvp + detects output redirection with '>'
int parse_command(char *buffer, char **args, char **outfile)
{
    int i = 0;
    char *token = strtok(buffer, " ");

    *outfile = NULL;

    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            *outfile = token;	//file after '>'
            return 1;   // redirection detected
        }
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return 0;
}

//Execute a command in a son process : if redirection is enabled, stdout is redirected to a file
void execute_command(char **args, int redirect, char *outfile,
                     int *last_exit, int *last_signal, long *elapsed_ms)
{
    int status = 0;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);	//start time measurement

    pid_t pid = fork();

    if (pid == 0) {		//son process
        if (redirect) {		// if redirection is enabled, stdout is redirected to a file
            int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0)
                exit(1);
            dup2(fd, STDOUT_FILENO); //redirect stdout to file
            close(fd);
        }

        execvp(args[0], args);	//execvp returns only if an error occurs
        write(STDERR_FILENO, "Command not found\n", 18);
        exit(1);
    }
    else if (pid > 0) {		//The parent process waits for the son and retrives :
				// exit code, termination signal, execution time
        wait(&status);

        clock_gettime(CLOCK_MONOTONIC, &end);	//stop time measurement
        *elapsed_ms = (end.tv_sec - start.tv_sec) * 1000
                    + (end.tv_nsec - start.tv_nsec) / 1000000;

	//Analyze son termination (exit or signal)
        if (WIFEXITED(status)) {
            *last_exit = WEXITSTATUS(status);
            *last_signal = -1;
        }
        else if (WIFSIGNALED(status)) {
            *last_signal = WTERMSIG(status);
        }
    }
}
