/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./io.h"
#include "./math.h"

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
  char clearer[2];
  backspace[0] = 0x8;
  backspace[1] = 0;

  clearer[0] = ' ';
  clearer[1] = 0;

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
        printString(clearer);
        printString(backspace);
      }
    } else {
      printString(buf + i);
      i++;
    }
  }
}


int itostring_c(char* buffer, int value, int base, int written) {
  /* Code taken from codepad.org/fUlU6thO */
  char letters[17];
  if ((value/base) > 0) {
    written = itostring_c(buffer, value/base, base, written);
  }
  letters[0] = '0';
  letters[1] = '1';
  letters[2] = '2';
  letters[3] = '3';
  letters[4] = '4';
  letters[5] = '5';
  letters[6] = '6';
  letters[7] = '7';
  letters[8] = '8';
  letters[9] = '9';
  letters[10] = 'A';
  letters[11] = 'B';
  letters[12] = 'C';
  letters[13] = 'D';
  letters[14] = 'E';
  letters[15] = 'F';
  letters[16] = '\0';

  buffer[written++] = (letters[mod(value, base)]);
  return written;
}

int itostring(char* buffer, int value, int base) {
  /* Code taken from codepad.org/fUlU6thO */
  int written;
  written = itostring_c(buffer, value, base, 0);
  buffer[written] = '\0';
  return written;
}


void printInt(int i) {
  char buf[3];
  itostring(buf, i, 10);
  printString(buf);
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

void readFile(char* fileName, char* buf) {
  int i, j;
  char dirSector[512];
  char* fname;
  char* found = 0;

  readSector(dirSector, 2);
  found = 0;
  for (i = 0; i < 16; i++) {
    fname = dirSector + (i * 32);

    for (j = 0; fname[j] == fileName[j] && j < 6; j++) {
    }
    if ((fname[j-1] == 0 || j == 6) && j >= 3) {
      /* Filename is correct */
      found = dirSector + (i * 32);
      break;
    }
  }

  if (found) {
    for (j = 6; j < 32; j++) {
      if (found[j] != 0) {
        readSector(buf, found[j]);
      } else {
        return;
      }
    }
  }
}

