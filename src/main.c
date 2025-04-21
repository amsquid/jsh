#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "shell_command.h"
#include "colors.h"

pid_t parentPid;
int debug = -1;

char username[128], hostname[128];

/*
 * Overwriting signals if needed to do nothing
 */
void handleSignal(int sig) { }

int start_menu() {
  printf("==== JSH ====\nSigned in as %s@%s\nPlease select an option from the menu below:\n1) Continue to shell\n2) Log out\n> ", username, hostname);
  char* option = malloc(1024);
  fgets(option, 1024, stdin);

  if(strcmp(option, "2\n") == 0) {
    free(option);
    return EXIT_CODE;
  }

  free(option);
  return 0;
}

int parseCommand(char* cmd) {
  int argc;
  char** argv = split(cmd, " ", &argc);
  char** formattedArgv;

  while(argv[argc] != NULL) argc++;
  for(int i = 0; i < argc; i++) { // env parsing
    char* arg = argv[i];
    if(arg[0] != '$') continue;
    char* envName = arg + 1;
    char* envValue = getenv(envName);
    argv[i] = envValue;
  }

  int out = parseShellCommand(argc, argv);
  if(out >= 0) return out;
  else runProgram(argv);

  free(argv);
  return 0;
}

int main(int argc, char* argv[]) {
  signal(SIGINT, handleSignal);

  gethostname(hostname, 128);
  getlogin_r(username, 128);

  if(argc > 1) {
    if(strcmp(argv[1], "debug") == 0) debug = 1;
    
  }

  if(argc == 1 || strcmp(argv[1], "nomenu") != 0) {
    int out = start_menu();
    if(out == EXIT_CODE) return 0;
  }

  parentPid = getpid();

  for(;;) {
    char command[1024] = "";
    char cwd[128];
    getcwd(cwd, 128);
 
    char* fmt = "%s%s@%s %s%s%s%%%s ";

    printf(fmt, BLUE, username, hostname, WHITE, cwd, BLUE, RESET);
    fgets(command, 1024, stdin);
    command[strlen(command) - 1] = '\0';
    int code = parseCommand(command);
    if(code == EXIT_CODE) break;
    command[0] = '\0';
  }

  //free(path);
}
