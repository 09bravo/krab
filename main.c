#include "input.h"
int main(int argc, char **argv) {
  signal(SIGINT, SIG_IGN);
  input();
  return 0;
}
