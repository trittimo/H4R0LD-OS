/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./interrupt.h"
#include "./shell.h"
#include "./io.h"
#include "./kernel.h"

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  switch (ax) {
    case 0x0:
      /* printString(buf) */
      printString(bx);
      break;
    case 0x1:
      /* readString(buf) */
      readString(bx);
      break;
    case 0x2:
      /* readSector(buf, sector) */
      readSector(bx, cx);
      break;
    case 0x3:
      /* readFile(fileName, buf) */
      readFile(bx, cx);
      break;
    case 0x4:
      /* executeProgram(name, segment) */
      executeProgram(bx, cx);
      break;
    case 0x5:
      terminate();
      break;
    default:
      printString("ax not handled!\n");
  }
}

void registerInterruptHandlers() {
  int i;

  char* addr = (char*) handleInterrupt21;

  for (i = 0; i < 4; i++) {
    putInMemory(0, 0x0084 + i, addr[i]);
  }
}
