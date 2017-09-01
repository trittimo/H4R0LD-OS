/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./kernel.h"
#include "./io.h"

int main(void) {
  char buffer[512];
  char line[80];
  char otherline[80];

  /* Register our interrupt handlers */
  registerInterruptHandlers();

  makeInterrupt21();

  interrupt(0x21, 0, "enter ur inputs kiddo: ", 0, 0);
  interrupt(0x21, 1, line, 0, 0);
  interrupt(0x21, 0, line, 0, 0);
  interrupt(0x21, 2, otherline, 30, 0);
  printString("We read from da sector, here's the message: ");
  printString(otherline);
  while (1) {}
  return 0;
}
