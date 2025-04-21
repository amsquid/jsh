#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#define EXIT_CODE 1000

char** split(char* str, char* delim, int* amount);
int run_program(char* argv[]);
int parse_shell_command(int argc, char* argv[]);

void clear_aliases();

#endif // !SHELL_COMMAND_H
