#ifndef UTILS_H
#define UTILS_H

void write_number(int n);
void display_prompt(int first_cmd, int last_exit, int last_signal, long elapsed_ms);
int parse_command(char *buffer, char **args, char **outfile);
void execute_command(char **args, int redirect, char *outfile,
                     int *last_exit, int *last_signal, long *elapsed_ms);

#endif
