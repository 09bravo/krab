#include "commands.h"
#include "prompt.h"
#include "raw_mode.h"
#include <stdio.h>
#include <unistd.h>
#include "history.h"
#include "wordexp.h"
int input() {
  enableRawMode();
  prompt();
  fflush(stdout);
  char buffer[1024];
  int len = 0;
  while (1) {
    char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0)
      continue;
    if (c == '\n' || c == '\r') {
      buffer[len] = '\0';
      printf("\r\n");
      if (len == 0) {
        prompt();
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
      if(wordexp(buffer, &p, 0) != 0) {
	fprintf(stderr, "krab: Illegal command \"%s\"\r\n", buffer);
	prompt();
	len = 0;
	fflush(stdout);
	continue;
      }
      args = p.we_wordv;
      if(wordexp(buffer, &p, 0) == 0) {
	args = p.we_wordv;
	if(args[0] == NULL) {
	wordfree(&p);
	continue;
	}
      } 
      buffer[strcspn(buffer, "\n")] = 0;
      if (strcmp(args[0], "cd") == 0) {
        cd(args[1]);
	wordfree(&p);
	prompt();
	len = 0;
	fflush(stdout);
        continue;
      }
   if (strcmp(args[0], "help") == 0) {
        help();
	wordfree(&p);
	prompt();
	len = 0;
	fflush(stdout);
        continue;
      }
      pid_t pid = fork();
      if (pid == 0) {
        disableRawMode();
	signal(SIGINT, SIG_DFL);
        execvp(args[0], args);
        printf("krab: Unrecognized command: %s\n", buffer);
        return 1;
      } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
	enableRawMode();
      } else {
        perror("fork");
      }
      len = 0;
      prompt();
      fflush(stdout);
    } else if (c == 4) {
      disableRawMode();
      break;
    } else if (c == 127 || c == '\b') {
      if (len > 0) {
        len--;
        printf("\b \b");
        fflush(stdout);
      }
    } else {
      if (len < sizeof(buffer) - 1) {
        buffer[len++] = c;
        printf("%c", c);
        fflush(stdout);
      }
    }
  }
  return 0;
}
