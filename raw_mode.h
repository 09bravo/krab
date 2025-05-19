#ifndef RAW_MODE_H
#define RAW_MODE_H
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
static struct termios orig_termios;
void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }
void enableRawMode() {
  struct termios raw = orig_termios;
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif
