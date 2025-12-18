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
	
	//Welcome Message
        write(STDOUT_FILENO, WELCOME, strlen(WELCOME));

        while (1) {
	    //Prompt Printing
            write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	    //Reading the command
            bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	    //Ctrl+D
	    if (bytes_read == 0){
		write(STDOUT_FILENO, "\n", 1);//line break -> clean display on the terminal
		write(STDOUT_FILENO, BYE, strlen(BYE));//Printing of the BYE message
		return 0;
	    }

            if (bytes_read <= 0) {
                continue;//error happened
	    }
	    //'\n' replacement with '\0' for strcmp
            buffer[bytes_read - 1] = '\0';

            if (strcmp(buffer, "exit") == 0){
		write(STDOUT_FILENO, BYE, strlen(BYE));
            	return 0; 
            }
	    //creation of the son processus
            pid_t pid = fork();
 
            if (pid == 0) {
		//Son processus 
                char *args[] = { buffer, NULL };
                execvp(buffer, args);
		//If exec fails
                write(STDERR_FILENO, "Command not found\n", strlen("Command not found\n"));
                exit(1);
            }
            else if (pid > 0) {
		//Father processus 
                wait(NULL);
            }
            else {
		write(STDERR_FILENO, "Fork error\n", strlen("Fork error\n"));
            }
    }

    return 0;
} 
