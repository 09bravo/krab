#include "input.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char** argv) {
  for(int i = 1; i < argc; i++) {
  if(strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
  puts("Krab, 1.0");
  return 0;
  }
  }
  signal(SIGINT, SIG_IGN);
  input();
  return 0;
}
