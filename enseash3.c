#include <unistd.h>
#include <string.h>  
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#define WELCOME "Welcome in the shell ENSEA. \nTo exit, type 'exit'.\n"
#define PROMPT  "enseash % "
#define BUFFER_SIZE 256
#define BYE "Bye bye ...\n"

int main(void){
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        write(STDOUT_FILENO, WELCOME, strlen(WELCOME));

         while (1) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

	if (bytes_read == 0){
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, BYE, strlen(BYE));
		return 0;
	}

        if (bytes_read <= 0) {
            continue;
	}
 
        if (buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
        }

	if (bytes_read == 0){
		continue;
	}

        if (strncmp(buffer, "exit", 4) == 0 && (bytes_read == 4 )){
		write(STDOUT_FILENO, BYE, strlen(BYE));
            	return 0; 
        }

        pid_t pid = fork();
 
        if (pid == 0) { 
            char *args[] = { buffer, NULL };
            execvp(buffer, args);

            write(STDOUT_FILENO, "Command not found\n", strlen("Command not found\n"));
            exit(1);
        }
        else if (pid > 0) { 
            wait(NULL);
        }
        else {
		write(STDOUT_FILENO, "Fork error\n", strlen("Fork error\n"));
        }
    }

    return 0;
} 
