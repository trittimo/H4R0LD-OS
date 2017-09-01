/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./kernel.h"
#include "./io.h"
#include "./interrupt.h"

int main(void) {
  char buffer[13312];
  char shell[6];

  /* LEAVE THIS IN FOR ALL FU`TURE KERNEL VERSIONS */
  registerInterruptHandlers();
  makeInterrupt21();
  
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  interrupt(0x21, 0x4, shell, 0x2000, 0);
  return 0;
}

void executeProgram(char* name, int segment) {
  int i;
  char buf[0x3400];

  if (mod(segment, 0x1000) != 0 || segment <= 1000 || segment >= 0xA000) {
    return;
  }

  readFile(name, buf);
  for (i = 0; i < 0x3400; i++) {
    putInMemory(segment, i, buf[i]);
  }

  launchProgram(segment);
}

void terminate() {
  char shell[6];
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  interrupt(0x21, 0x4, shell, 0x2000, 0);
}
