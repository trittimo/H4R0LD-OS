/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./kernel.h"
#include <math.h>
#include <string.h>

int main(void) {
  char buffer[13312];
  char shell[6];

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
    case 0x6:
      /* void writeSector(char* buf, int sector) */
      writeSector(bx, cx);
      break;
    case 0x7:
      deleteFile(bx);
      break;
    case 0x8:
      /* writeFile(char* name, char* buf, int numberOfSectors) */
      writeFile(bx, cx, dx);
      break;
    case 0x99:
      printInt(bx);
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

void writeSector(char* buf, int sector) {
  int ah = 3;
  int al = 1;
  int bx = (int) buf;
  int ch = div(sector, 36);
  int cl = mod(sector, 18) + 1;
  int dh = mod(div(sector, 18), 2);
  int dl = 0;

  interrupt(0x13, ah*256 + al, bx, ch*256 + cl, dh*256 + dl);
}

char* getFileSectors(char* filename, char* dir) {
  int i, j;
  for (i = 0; i < 16; i++) {
    for (j = 0; dir[i * 32 + j] == filename[j] && j < 6; j++) {
      if (filename[j] == 0) {
        break;
      }
    }
    if ((j == 6 || dir[i * 32 + j] == 0) && j >= 3) {
      return dir + i * 32;
    }
  }
  return 0;
}

char* getEmptyBlock(char* dir) {
  int i;
  for (i = 0; i < 16; i++) {
    if (!dir[i*32]) {
      /* Empty spot for file */
      return dir + i * 32;
    }
  }
  return 0;
}

void readFile(char* fileName, char* buf) {
  int i;
  char dirSector[512];
  char* found;

  readSector(dirSector, 2);
  found = getFileSectors(fileName, dirSector);

  if (found) {
    for (i = 6; i < 32; i++) {
      if (found[i] != 0) {
        readSector(buf + (i - 6) * 512, found[i]);
      } else {
        return;
      }
    }
  }
}

void deleteFile(char* fileName) {
  int i;
  char mapSector[512];
  char dirSector[512];
  char* found;

  readSector(mapSector, 1);
  readSector(dirSector, 2);

  found = getFileSectors(fileName, dirSector);

  if (found) {
    found[0] = 0;
    for (i = 6; i < 32; i++) {
      if (found[i] != 0) {
        mapSector[found[i]] = 0;
      } else {
        break;
      }
    }
  } else {
    return;
  }

  writeSector(mapSector, 1);
  writeSector(dirSector, 2);
}

void writeFile(char* name, char* buf, int numberOfSectors) {
  int i, j, sector;
  char mapSector[512];
  char dirSector[512];
  char sectors[16];
  char* found;
  char c[2];
  c[0] = ' ';
  c[1] = 0;

  readSector(mapSector, 1);
  readSector(dirSector, 2);

  found = getEmptyBlock(dirSector);
  if (!found) {
    return;
  }

  for (i = 0; i < 6; i++) {
    /* Write in the filename */
    if (name[i] == '\n' || name[i] == '\r') {
      found[i] = 0;
      continue;
    }

    found[i] = name[i];

    if (name[i] == 0) {
      break;
    }
  }

  sector = 0;
  for (i = 0; i < 512; i++) {
    if (mapSector[i] == 0 && sector < numberOfSectors) {
      mapSector[i] = 0xFF;
      found[sector + 6] = i;
      writeSector(buf + sector * 512, i);
      sector++;
    }
  }

  for (i = sector; i < 26; i++) {
    found[sector + 6] = 0;
  }

  writeSector(mapSector, 1);
  writeSector(dirSector, 2);
}