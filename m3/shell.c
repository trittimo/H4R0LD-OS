#include "./shell.h"
#include "./string.h"
#include "./mem.h"

int main(void) {
  char command[128];

  while (1) {
    interrupt(0x21, 0x0, "H4R0LD> \0", 0, 0);
    interrupt(0x21, 0x1, command, 0, 0);
    runCommand(command);
  }
  return 0;
}

void runCommand(char* command) {
  char buf[0x3000];
  zero(buf, 0x3000);
  if (strncmp(command, "type ", 5)) {
    interrupt(0x21, 0x3, command + 5, buf, 0);
    interrupt(0x21, 0x0, buf, 0, 0);
  } else if (strncmp(command, "execute ", 8)) {
    interrupt(0x21, 0x4, command + 8, 0x2000, 0);
  }
}
