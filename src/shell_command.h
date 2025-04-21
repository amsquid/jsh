#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#define EXIT_CODE 1000

char** split(char* str, char* delim, int* amount);
int runProgram(char* argv[]);
int parseShellCommand(int argc, char* argv[]);

void clear_aliases();

#endif // !SHELL_COMMAND_H
