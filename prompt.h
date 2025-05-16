#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
const char *getUserName() {
  struct passwd *pw = getpwuid(geteuid());
  return pw ? pw->pw_name : "unknown";
}
void prompt() {
  char hostname[HOST_NAME_MAX + 1];
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  char *display_path = cwd;
  char *homedir = getenv("HOME");
  if (homedir && strncmp(cwd, homedir, strlen(homedir)) == 0) {
    static char short_path[PATH_MAX];
    snprintf(short_path, sizeof(short_path), "~%s", cwd + strlen(homedir));
    display_path = short_path;
  }
  const char *username = getUserName();
  gethostname(hostname, HOST_NAME_MAX + 1);
  if (username && strcmp(username, "root") == 0) {
    printf("\r{%s@%s %s}# ", username, hostname, display_path);
  } else {
    printf("\r{%s@%s %s}$ ", username, hostname, display_path);
  }
}
