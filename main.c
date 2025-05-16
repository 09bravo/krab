#include "input.h"
#include <linux/limits.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char **argv) {
  signal(SIGINT, SIG_IGN);
  input();
  return 0;
}
