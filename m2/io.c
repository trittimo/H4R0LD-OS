/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./io.h"
#include "./math.h"

#define VID_MEM_SEGMENT 0xB000
#define VID_MEM_OFFSET 0x8000

void clear() {
  int x, y;
  for (y = 0; y < 25; y++) {
    for (x = 0; x < 80; x++) {
      print(" ", 1, x, y);
    }
  }
}

void print(char* string, int length, int x, int y) {
  int i, offset;
  offset = 80 * y;
  offset *= 2;
  offset += x * 2;
  offset += VID_MEM_OFFSET;
  for (i = 0; i < length; i++) {
    putInMemory(VID_MEM_SEGMENT, offset+(i*2), string[i]);
    putInMemory(VID_MEM_SEGMENT, offset+(i*2)+1, 0x7);
  }
}

void printString(char* string) {
  int i;
  char al, ah;
  int ax;

  for (i = 0; ; i++) {
    if (string[i] == 0) {
      return;
    }
    al = string[i];
    ah = 0xe;
    ax = ah * 256 + al;
    interrupt(0x10, ax, 0, 0, 0);
  }
}

void readString(char* buf) {
  int r;
  int i = 0;
  char backspace[2];
  backspace[0] = 0x8;
  backspace[1] = 0;

  while (1) {
    r = interrupt(0x16, 0, 0, 0, 0);
    buf[i] = r;
    buf[i+1] = 0;
    if (r == 0xd) { /* If we get a newline */
      buf[++i] = 0xa; /* Add a line feed */
      buf[++i] = 0; /* Null terminate */
      printString(buf + i - 2);
      return;
    } else if (r == 0x8) { /* Backspace */
      if (i > 0) {
        i--;
        printString(backspace);
        printString(" ");
        printString(backspace);
      }
    } else {
      printString(buf + i);
      i++;
    }
  }
}

/*
the interrupt number, and the interrupt parameters passed in the AX, BX, CX, and DX registers

    AH = 2 (this number tells the BIOS to read a sector as opposed to write)
    AL = number of sectors to read (use 1)
    BX = address where the data should be stored (pass your char * array here)
    CH = track number
    CL = relative sector number
    DH = head number
    DL = device number (for the floppy disk, use 0)

    ax = AH*256 + AL

 */
void readSector(char* buf, int sector) {
  int ah = 2;
  int al = 1;
  int bx = (int) buf;
  int ch = div(sector, 36);
  int cl = mod(sector, 18) + 1;
  int dh = mod(div(sector, 18), 2);
  int dl = 0;

  interrupt(0x13, ah*256 + al, bx, ch*256 + cl, dh*256 + dl);
}
