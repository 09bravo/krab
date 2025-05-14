#include <linux/limits.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "input.h"
int main(int argc, char **argv) {
  signal(SIGINT, SIG_IGN);
  input();
  return 0;
}
