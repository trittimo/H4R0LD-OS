#include "./shell.h"
#include <string.h>
#include <mem.h>

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
  int fsize;
  zero(buf, 0x3000);
  if (strncmp(command, "type ", 5)) {
    interrupt(0x21, 0x3, command + 5, buf, 0);
    interrupt(0x21, 0x0, buf, 0, 0);
  } else if (strncmp(command, "execute ", 8)) {
    interrupt(0x21, 0x4, command + 8, 0x2000, 0);
  } else if (strncmp(command, "copy ", 5)) {
    int pos = strpos(' ', command + 5);
    if (pos == -1) {
      interrupt(0x21, 0x0, "Usage:\n\r\tcopy [in] [out]\n\r", 0, 0);
      return;
    }
    interrupt(0x21, 0x3, command + 5, buf, 0, 0);
    fsize = getFileSize(command + 5);
    interrupt(0x21, 0x0, "Copied ", 0, 0);
    interrupt(0x21, 0x99, fsize, 0, 0);
    interrupt(0x21, 0x0, " sectors\n\r");
    interrupt(0x21, 0x8, command + 5 + pos + 1, buf, fsize);
  } else if (strncmp(command, "delete ", 7)) {
    interrupt(0x21, 0x7, command + 7, 0, 0);
  } else if (strncmp(command, "dir", 3)) {
    interrupt(0x21, 0x2, buf, 2);
    listDirectory(buf);
  } else if (strncmp(command, "create ", 7)) {
    createFile(command + 7, buf);
  }
}

int getFileSize(char* filename) {
  int i, j, sectors;
  char dir[512];
  interrupt(0x21, 0x2, dir, 2, 0);
  sectors = 0;
  for (i = 0; i < 6; i++) {
    if (filename[i] == ' ') {
      filename[i] = 0;
    }
  }
  for (i = 0; i < 512; i+=32) {
    if (strncmp(filename, dir + i, 6)) {
      for (sectors = 0; sectors < 26 && dir[i+6+sectors] != 0; sectors++) {
      }
    }
  }
  return sectors;
}

void listDirectory(char* dir) {
  int i, sectors;
  for (i = 0; i < 512; i+=32) {
    if (dir[i]) {
      for (sectors = 0; sectors < 26 && dir[i+6+sectors] != 0; sectors++) {
      }
      dir[i+6] = 0;
      interrupt(0x21, 0x0, dir + i, 0, 0);
      interrupt(0x21, 0x0, "\t -> ", 0, 0);
      interrupt(0x21, 0x99, sectors, 0, 0);
      interrupt(0x21, 0x0, " sectors\n\r", 0, 0);
    }
  }
}

void createFile(char* filename, char* file) {
  int len = 0;
  int tl = 0;
  while (1) {
    interrupt(0x21, 0x1, file + len, 0, 0);
    tl = strlen(file + len);
    if (tl == 2) {
      file[len + 1] = 0;
      file[len + 2] = 0;
      break;
    }
    len += tl;
  }
  interrupt(0x21, 0x8, filename, file, div(len,512) + 1);
}