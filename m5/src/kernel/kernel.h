/*
  Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
  Team L
*/

#ifndef _KERNEL_H
#define _KERNEL_H

/* Milestone 5 */
void listDirectory(char* dir);
int executeProgram(char* name);
void handleTimerInterrupt(int segment, int sp);
void terminate();
void killProcess(int process);
void setupWait(char* fileName);

typedef struct Process {
  int active;
  int stackPointer;
  int waiting;
} Process;

/* Milestone 4 */
void writeSector(char* fileName, char* buf);
void deleteFile(char* fileName);
void writeFile(char* name, char* buf, int numberOfSectors);
char* getFileSectors(char* filename, char* dirSector);
char* getEmptyBlock(char* dirSector);

/* Milestone 3 */
void readFile(char* fileName, char* buf);
void printInt(int i);
int itostring_c(char* buffer, int value, int base, int written);
int itostring(char* buffer, int value, int base);

/* Milestone 2 */
void printString(char* string);
void readString(char* buf);
void readSector(char* buf, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);

/* Milestone 1 */
void print(char* string, int x, int y);
void clear();

#endif