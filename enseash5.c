#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

#define WELCOME "Welcome in the shell ENSEA. \nTo exit, type 'exit'.\n"
#define PROMPT  "enseash % "
#define BUFFER_SIZE 256
#define BYE "Bye bye ...\n"

void write_number(int n)
{
    char c;
    if (n >= 10) write_number(n / 10);
    c = '0' + (n % 10);
    write(STDOUT_FILENO, &c, 1);
}

int main(void)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    int status = 0;
    int last_exit = 0;
    int last_signal = -1;
    int first_cmd = 1;
    long elapsed_ms = 0;

    struct timespec start, end;

    write(STDOUT_FILENO, WELCOME, strlen(WELCOME));

    while (1) {
	//Prompt display with the previous status and time
        if (first_cmd == 0) {
            if (last_signal != -1) {
                write(STDOUT_FILENO, "[sign:", 6);
                write_number(last_signal);
                write(STDOUT_FILENO, "|", 1);
                write_number(elapsed_ms);
                write(STDOUT_FILENO, "ms] ", 4);
            } else {
                write(STDOUT_FILENO, "[exit:", 6);
                write_number(last_exit);
                write(STDOUT_FILENO, "|", 1);
                write_number(elapsed_ms);
                write(STDOUT_FILENO, "ms] ", 4);
            }
        }

        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes_read == 0) {
            write(STDOUT_FILENO, "\n", 1);
            write(STDOUT_FILENO, BYE, strlen(BYE));
            return 0;
        }

        if (bytes_read < 0) continue;

        buffer[bytes_read - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            write(STDOUT_FILENO, BYE, strlen(BYE));
            return 0;
        }

        // Beginning of the clock
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid_t pid = fork();

        if (pid == 0) {
            char *args[] = { buffer, NULL };
            execvp(buffer, args);
            write(STDERR_FILENO, "Command not found\n", 18);
            exit(1);
        }
        else if (pid > 0) {
            wait(&status);

            // End of the clock
            clock_gettime(CLOCK_MONOTONIC, &end);
            elapsed_ms = (end.tv_sec - start.tv_sec) * 1000
                       + (end.tv_nsec - start.tv_nsec) / 1000000;

            if (WIFEXITED(status)) {
                last_exit = WEXITSTATUS(status);
                last_signal = -1;
            } else if (WIFSIGNALED(status)) {
                last_signal = WTERMSIG(status);
            }

            first_cmd = 0;
        }
        else {
            write(STDERR_FILENO, "Fork error\n", 11);
        }
    }
}

