#include <limits.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "commands.h"
const char *getUserName() {
  struct passwd *pw = getpwuid(geteuid());
  return pw ? pw->pw_name : "unknown";
}

void prompt() {
  char hostname[HOST_NAME_MAX + 1];
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  char* display_path = cwd;
  char* homedir = getenv("HOME");
  if(homedir && strncmp(cwd, homedir, strlen(homedir)) == 0) {
	static char short_path[PATH_MAX];
	snprintf(short_path, sizeof(short_path), "~%s", cwd + strlen(homedir));
	display_path = short_path;
  }
  const char *username = getUserName();
  gethostname(hostname, HOST_NAME_MAX + 1);
  if (username && strcmp(username, "root") == 0) {
    printf("{%s@%s %s}# ", username, hostname, display_path);
  } else {
    printf("{%s@%s %s}$ ", username, hostname, display_path);
  }
}

int main(int argc, char **argv) {
  signal(SIGINT, SIG_IGN);
  char buffer[1024];
  while (1) {
    prompt();
    if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
	printf("\n");
	break;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    if(strcmp(buffer, "exit") == 0) {
	break;	
    }
    buffer[strcspn(buffer, "\n")] = 0;
    char *args[100];
    int i = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL && i < 99) {
	args[i++] = token;
	token = strtok(NULL, " ");
    }
    args[i] = NULL;
    if (args[0] == NULL) {
	continue;
    }
    if (strcmp(args[0], "cd") == 0) { 
    cd(args[1]);
    continue;
    }  
    pid_t pid = fork();
    if (pid == 0) {
	signal(SIGINT, SIG_DFL);
	execvp(args[0], args);
	printf("dish: Unrecognized command: %s\n", buffer);
	return 1;
    } else if (pid > 0) {
	int status;
	waitpid(pid, &status, 0);
    } else {
	perror("fork");
    }
  }
  return 0;
}
