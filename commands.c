#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
int cd(char *path) {
  if (path == NULL) {
    path = getenv("HOME");
  }
  if (chdir(path) != 0) {
    printf("krab: \"%s\" no such a directory\r\n", path);
    return 1;
  }
  return 0;
}
int help() {
  puts("olight0's krab, by the community for community.\r\n");
  puts("the help command is currently still on the work.\r");
  return 0;
}
