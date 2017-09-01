/*
  Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
  Team L
*/
#include "./kernel.h"
#include <math.h>
#include <mem.h>
#include <string.h>

Process processes[8];
int currentProcess;

int main(void) {
  int i;
  char buffer[13312];
  char shell[6];

  setKernelDataSegment();
  /* Setup our processes */
  for (i = 0; i < sizeof(processes); i++) {
    processes[i].active = 0;
    processes[i].stackPointer = 0xff00;
    processes[i].waiting = -1;
  }

  restoreDataSegment();

  currentProcess = 0;

  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';

  makeInterrupt21();
  makeTimerInterrupt();
  executeProgram(shell);

  while (1) {
    
  }
  return 0;
}

void handleTimerInterrupt(int segment, int sp) {
  int i, c;
  setKernelDataSegment();
  c = currentProcess;
  if ((segment >= 0x2000) && (segment <= 0x9000)) {
    processes[c].stackPointer = sp;
  }

  for (i = mod(c + 1, 8); i != c; ) {
    if (processes[i].waiting != -1 &&
      (processes[processes[i].waiting].active == 0)) {
      /* If the process we're waiting on is already dead, */
      /* set the waiting value back to -1 */
      processes[i].waiting = -1;
    } else if (processes[i].waiting != -1) {
    
    }
    if (processes[i].active && processes[i].waiting == -1) {
      /* If the process is active, we will save its segment and sp */
      segment = (i + 2) * 0x1000;
      sp = processes[i].stackPointer;
      break;
    }
    i = mod(i + 1, 8);
  }
  
  if ((i == c) && (processes[processes[i].waiting].active == 0)) {
    processes[i].waiting = -1;
  }

  /* Check if the currently running process is the only one to run */
  if ((i == c) && (processes[i].active) && (processes[i].waiting == -1)) {
    segment = (i + 2) * 0x1000;
    sp = processes[c].stackPointer;
  }

  currentProcess = i;
  restoreDataSegment();
  returnFromTimer(segment, sp);
}

int executeProgram(char* name) {
  int i, j, segment, inactive = 0;
  char buf[0x3400];
  setKernelDataSegment();
  for (i = 0; i < sizeof(processes); i++) {
    if (!processes[i].active) {
      inactive = i;
      break;
    }
  }
  restoreDataSegment();
  segment = (i + 2) * 0x1000;
  readFile(name, buf);
  for (j = 0; j < 0x3400; j++) {
    putInMemory(segment, j, buf[j]);
  }
  setKernelDataSegment();
  processes[inactive].active = 1;
  processes[inactive].stackPointer = 0xff00;
  restoreDataSegment();
  initializeProgram(segment);
  return i;
}

void terminate() {
  setKernelDataSegment();
  killProcess(currentProcess);
  restoreDataSegment();
  while (1) {
  }
}

void killProcess(int process) {
  int i;
  setKernelDataSegment();
  processes[process].active = 0;
  for (i = 0; i < sizeof(processes); i++) {
    processes[i].waiting = -1;
  }
  restoreDataSegment();
}

void setupWait(char* name) {
  int i, process = executeProgram(name);
  setKernelDataSegment();
  for (i = 0; i < sizeof(processes); i++) {
    if (i != process) {
      processes[i].waiting = process;
    }
  }
  restoreDataSegment();
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

void listDirectory(char* dir) {
  char sectorsStr[11];
  char tab[6];
  int i, sectors;
  sectorsStr[0] = ' ';
  sectorsStr[1] = 's';
  sectorsStr[2] = 'e';
  sectorsStr[3] = 'c';
  sectorsStr[4] = 't';
  sectorsStr[5] = 'o';
  sectorsStr[6] = 'r';
  sectorsStr[7] = 's';
  sectorsStr[8] = '\n';
  sectorsStr[9] = '\r';
  sectorsStr[10] = '\0';
  tab[0] = '\t';
  tab[1] = ' ';
  tab[2] = '-';
  tab[3] = '>';
  tab[4] = ' ';
  tab[5] = '\0';
  for (i = 0; i < 512; i+=32) {
    if (dir[i]) {
      for (sectors = 0; sectors < 26 && dir[i+6+sectors] != 0; sectors++) {
      }
      dir[i+6] = 0;
      interrupt(0x21, 0x0, dir + i, 0, 0);
      interrupt(0x21, 0x0, tab, 0, 0);
      interrupt(0x21, 0x99, sectors, 0, 0);
      interrupt(0x21, 0x0, sectorsStr, 0, 0);
    }
  }
}

void clear() {
  int i;
  char newline[3];
  newline[0] = '\r';
  newline[1] = '\n';
  newline[2] = 0;
  for (i = 0; i < 24; i++) {
    interrupt(0x21, 0x0, newline, 0, 0);
  }
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
      executeProgram(bx);
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
    case 0x9:
      killProcess(bx);
      break;
    case 0x96:
      clear();
      break;
    case 0x97:
      setupWait(bx);
      break;
    case 0x98:
      listDirectory(bx);
      break;
    case 0x99:
      printInt(bx);
      break;
    default:
      printString("ax not handled!\n");
  }
}