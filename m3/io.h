/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#ifndef _PRINT_H
#define _PRINT_H

/* Milestone 3 */
void readFile(char* fileName, char* buf);
void printInt(int i);
int itostring_c(char* buffer, int value, int base, int written);
int itostring(char* buffer, int value, int base);

/* Milestone 2 */
void printString(char* string);
void readString(char* buf);
void readSector(char* buf, int sector);

/* Milestone 1 */
void print(char* string, int x, int y);
void clear();

#endif