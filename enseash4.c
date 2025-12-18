#include <unistd.h>  
#include <string.h>  
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

#define WELCOME "Welcome in the shell ENSEA. \nTo exit, type 'exit'.\n"
#define PROMPT  "enseash % "
#define BUFFER_SIZE 256    
#define BYE "Bye bye ...\n"

void write_number(int n){//recursive fct  -> displays the digit one by one
    char c;
    if (n >= 10){ write_number(n / 10);}
    c = '0' + (n % 10);
    write(STDOUT_FILENO, &c, 1);
}

int main(void){
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        
	int status = 0;
	int last_exit = 0;
	int last_signal = -1;
	int first_cmd = 1;

        //Welcome Message
        write(STDOUT_FILENO, WELCOME, strlen(WELCOME));

        while (1) {
	    if(first_cmd==0){
	    if (last_signal != -1) {
   		 write(STDOUT_FILENO, "[sign:", 6);
   		 write_number(last_signal);
   		 write(STDOUT_FILENO, "] ", 2);
	   }
	   else if (last_exit != -1) {
    		write(STDOUT_FILENO, "[exit:", 6);
    		write_number(last_exit);
    		write(STDOUT_FILENO, "] ", 2);
	   }
	   }
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
		wait(&status); 
                if (WIFEXITED(status)) {
		    last_exit = WEXITSTATUS(status);
   		    last_signal = -1;
		}
		else if (WIFSIGNALED(status)) {
    		    last_signal = WTERMSIG(status);
		    last_exit = -1;
		}
		first_cmd = 0; 
            }
            else {
                write(STDERR_FILENO, "Fork error\n", strlen("Fork error\n"));
            }
    }

    return 0;
} 

