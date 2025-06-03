#include "input.h"
#include <signal.h>
int main() {
  signal(SIGINT, SIG_IGN);
  input();
  return 0;
}
