#include "input.h"
#include "commands.h"
#include "history.h"
#include "prompt.h"
#include "raw_mode.h"
#include "syntx.h"
#include <stdlib.h>
#include <wordexp.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int input() {
  enableRawMode();
  printf("%s", prompt());
  fflush(stdout);
  size_t buffer_size = 1024;
  char *buffer = malloc(buffer_size);
  if(!buffer) {
    perror("malloc");
    return 1;
  }
  int len = 0;
  int cursor = 0;
  while (1) {
    char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0)
      continue;
    if (c == '\x1b') {
      char seq[2];
      if (read(STDIN_FILENO, &seq[0], 1) == 0)
        continue;
      if (read(STDIN_FILENO, &seq[1], 1) == 0)
        continue;
      if (seq[0] == '[') {
        if (seq[1] == 'C') {
          if (cursor < len) {
            cursor++;
            printf("\x1b[C");
            fflush(stdout);
          }
        } else if (seq[1] == 'D') {
          if (cursor > 0 && len > 0) {
            cursor--;
            printf("\x1b[D");
            fflush(stdout);
          }
        }
      }
      continue;
    } 
    if (c == '\n' || c == '\r') {
      buffer[len] = '\0';
      printf("\r\n");
      if (len == 0) {
        len = 0;
        cursor = 0;
        printf("%s", prompt());
        fflush(stdout);
        continue;
      }
      if (strcmp(buffer, "exit") == 0) {
        disableRawMode();
        break;
      }
      history(buffer);
      wordexp_t p;
      char **args = NULL;
      buffer[strcspn(buffer, "\n")] = 0;
      if (wordexp(buffer, &p, WRDE_NOCMD) != 0) {
        fprintf(stderr, "krab: Illegal command \"%s\"\r\n", buffer);
        printf("%s", prompt());
        len = 0;
        cursor = 0;
        fflush(stdout);
        continue;
      }
      args = p.we_wordv;
      if (args == NULL || args[0] == NULL) {
        wordfree(&p);
        fprintf(stderr, "krab: Hello");
        printf("%s", prompt());
        len = 0;
        cursor = 0;
        fflush(stdout);
        continue;
      }
      if (strcmp(args[0], "cd") == 0) {
        cd(args[1]);
        wordfree(&p);
        printf("%s", prompt());
        len = 0;
        cursor = 0;
        fflush(stdout);
        continue;
      }
      if (strcmp(args[0], "help") == 0) {
        help();
        wordfree(&p);
        printf("%s", prompt());
        len = 0;
        cursor = 0;
        fflush(stdout);
        continue;
      }
      pid_t pid = fork();
      if (pid == 0) {
        disableRawMode();
        signal(SIGINT, SIG_DFL);
        execvp(args[0], args);
        printf("krab: Unrecognized command: %s\n", buffer);
        wordfree(&p);
        free(buffer);
        return 1;
      } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        enableRawMode();
      } else {
        perror("fork");
      }
      len = 0;
      cursor = 0;
      printf("%s", prompt());
      fflush(stdout);
    } else if (c == 4) {
      disableRawMode();
      break;
    } else if (c == 12) {
      printf("\033[2J\033[H");
        printf("\r\x1b[2K");
      printf("%s", prompt());
        highlight(prompt(), buffer);
        printf("\x1b[%dG", visible_length(prompt()) + cursor + 1);
      fflush(stdout);
    } else if (c == 127 || c == '\b') {
      if (cursor > 0 && len > 0) {
        cursor--;
        memmove(&buffer[cursor], &buffer[cursor + 1], len - cursor);
        len--;
        buffer[len] = '\0';
        printf("\r\x1b[2K");
        highlight(prompt(), buffer);
        printf("\x1b[%dG", visible_length(prompt()) + cursor + 1);
        fflush(stdout);
      }
    } else {
      if ((size_t)(len + 1) >= buffer_size) {
        buffer_size *= 2;
        char *new_buffer = realloc(buffer, buffer_size);
        if(!new_buffer) {
          free(buffer);
          perror("realloc");
          return 1;
        }
        buffer = new_buffer;
        }
        memmove(&buffer[cursor + 1], &buffer[cursor], len - cursor);
        buffer[cursor] = c;
        len++;
        cursor++;
        buffer[len] = '\0';
        printf("\r\x1b[2K");
        highlight(prompt(), buffer);
        printf("\x1b[%dG", visible_length(prompt()) + cursor + 1);
        fflush(stdout);
      }
    }
   return 0;
}
