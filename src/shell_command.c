#include "shell_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

char** aliasNames;
char** aliasValues;
int aliases = 0;

char** split(char* str, char* delim, int* amount) {
  int initial = 4;
  char** out = malloc(sizeof(char*) * initial);

  char* item = malloc(strlen(str));
  strcpy(item, str);
  strtok(item, delim);

  int i = 0;
  while(item != NULL) {
    unsigned long size = strlen(item);
    if(i > initial)
      out = realloc(out, sizeof(char*) * (i + 1));
    out[i] = malloc(size + 1);
    strcpy(out[i], item);
    i++;
    item = strtok(NULL, delim);
  }

  out = realloc(out, sizeof(out) + sizeof(NULL));
  out[i] = NULL;
 
  *amount = i;
  return out;
}

int alias(int argc, char* argv[]) {
  if(argc < 3) {
    printf("alias <alias> <command>\n");
    return 0;
  }

  char* aliasName = argv[1];
  int size = strlen(argv[2]);
  char* command = malloc(size);
  strcpy(command, argv[2]);
  
  int i = 3;
  
  while(argv[i] != NULL) {
    int currSize = size;
    size += strlen(argv[i]) + 1; // extra 1 for a space
    command = realloc(command, size);
    strcat(command, " ");
    strcat(command, argv[i]);
    i++;
  }
  aliases++;

  aliasNames = realloc(aliasNames, sizeof(char*) * aliases);
  aliasValues = realloc(aliasValues, sizeof(char*) * aliases);

  aliasNames[aliases - 1] = malloc(strlen(aliasName));
  strcpy(aliasNames[aliases - 1], aliasName);

  aliasValues[aliases - 1] = malloc(strlen(command));
  strcpy(aliasValues[aliases - 1], command);
  
  return 0;
}

int cd(int argc, char* argv[]) {
  if(argc == 1) {
    uid_t uid = getuid();
    struct passwd* pwd = getpwuid(uid);
    if(!pwd) {
      printf("Failed to get pwdid for uid %d\n", uid);
      return -1;
    }
    char* home = pwd->pw_dir;
    chdir(home);
    return 0;
  }

  int out = chdir(argv[1]);

  if(out != 0) {
    printf("Failed to change directory to %s\n", argv[1]);
    return 0;
  }

  return 0;
}

int run_program(char* argv[]) {
  int status;
  pid_t wpid;
  pid_t pid = fork();

  if(pid == -1) {
    printf("Failed to create child process\n");
    return -1;
  }

  if(pid == 0) {
    if(execvp(argv[0], argv) == -1) {
      printf("Failed to execute command\n");
      exit(-1);
    }
    exit(0);
  } else if(pid > 0) {
    wait(NULL);
  }

  return 0;
}

int set(int argc, char* argv[]) {
  if(argc != 3) {
    printf("Sets an environment variable\nset <name> <value>\n");
    return -1;
  }

  char* name = argv[1];
  char* value = argv[2];

  setenv(name, value, 1);

  return 0;
}


int parse_shell_command(int argc, char* argv[]) {
  if(strcmp(argv[0], "exit") == 0) {
    return EXIT_CODE;
  }

  if(strcmp(argv[0], "cd") == 0) {
    return cd(argc, argv);
  }

  if(strcmp(argv[0], "alias") == 0) {
    return alias(argc, argv);
  }

  if(strcmp(argv[0], "set") == 0) {
    return set(argc, argv);
  }

  // Check for alias
  for(int i = 0; i < aliases; i++) {
    if(strcmp(aliasNames[i], argv[0]) == 0) {
      int am = 0;
      char** splitArgs = split(aliasValues[i], " ", &am);
      run_program(splitArgs);
      free(splitArgs);
      return 0;
    }
  }

  return -1;
}

void clear_aliases() {
  for(int i = 0; i < aliases; i++) {
    free(aliasNames[i]);
    free(aliasValues[i]);
  }

  free(aliasNames);
  free(aliasValues);
}

