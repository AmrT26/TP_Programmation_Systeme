#include <unistd.h>
#include <string.h>

#define WELCOME "Welcome in the shell ENSEA. \nTo exit, type 'exit'.\n"
#define PROMPT  "enseash % "

int main(void){
	write(STDOUT_FILENO, WELCOME, strlen(WELCOME));
	write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	return 0 ;
}


