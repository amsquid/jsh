#include "shell_command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

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

int set(int argc, char* argv[]) {
  if(argc == 1) {

  }

  return 0;
}

int cd(int argc, char* argv[]) {
  if(argc == 1 || strcmp(argv[1], "~") == 0) {
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

int parseShellCommand(int argc, char* argv[]) {
  if(strcmp(argv[0], "exit") == 0) {
    return EXIT_CODE;
  }

  if(strcmp(argv[0], "cd") == 0) {
    return cd(argc, argv);
  }

  if(strcmp(argv[0], "set") == 0) {
    return set(argc, argv);
  }

  return -1;
}

