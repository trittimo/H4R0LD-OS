#include "./interrupt.h"
#include "./io.h"

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  if (ax == 0) {
    printString(bx);
  } else if (ax == 1) {
    readString(bx);
  } else if (ax == 2) {
    readSector(bx, cx);
  } else {
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
