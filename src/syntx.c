#include "syntx.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int visible_length(const char *s) {
  int len = 0;
  while(*s) {
    if(*s == '\x1b') {
      while(*s && *s != 'm') s++;
      if(*s == 'm') s++;
    } else {
      len++;
      s++;
    }
  }
  return len;
}
int command_check(const char *cmd) {
  if (!strcmp(cmd, "cd") || !strcmp(cmd, "help") || !strcmp(cmd, "exit"))
    return 1;
  char *path = getenv("PATH");
  if (!path)
    return 0;
  char *paths = strdup(path);
  char *dir = strtok(paths, ":");
  while (dir) {
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd);
    if (access(fullpath, X_OK) == 0) {
      free(paths);
      return 1;
    }
    dir = strtok(NULL, ":");
  }
  free(paths);
  return 0;
}
void highlight(const char *prompt, const char *buf) {
  printf("\r\x1b[2K%s", prompt);
  const char *p = buf;
  char word[256];
  int first = 1;
  while (*p) {
    while (*p == ' ')
      putchar(*p++);
    int wlen = 0;
    if (*p == '"' || *p == '\'') {
      char qoute = *p;
      word[wlen++] = *p++;
      while (*p && *p != qoute && wlen < 255)
        word[wlen++] = *p++;
      if (*p == qoute && wlen < 255)
        word[wlen++] = *p++;
      if (wlen >= 255)
        wlen = 255;
      word[wlen] = '\0';
    } else {
      while (*p && *p != ' ' && wlen < 255) {
        word[wlen++] = *p++;
      }
      if (wlen >= 255)
        wlen = 255;
      word[wlen] = '\0';
    }
    if (first) {
      first = 0;
      if (command_check(word))
        printf(BOLD LIGHT_ORANGE "%s" RESET, word);
      else
        printf(ORANGE "%s" RESET, word);
    } else if (((word[0]) == '"' && word[wlen - 1] == '"') ||
               ((word[0] == '\'' && word[wlen - 1] == '\''))) {
      printf(MAGENTA "%s" RESET, word);
    } else if (word[0] == '-') {
      printf(MAGENTA "%s" RESET, word);
    } else {
      printf("%s", word);
    }
  }
  printf("\x1b[K");
  fflush(stdout);
}
