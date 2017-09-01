#ifndef _SHELL_H
#define _SHELL_H

void executeProgram(char* name, int segment);
void runCommand(char* command);
void listDirectory(char* dir);
void createFile(char* filename, char* buf);
int getFileSize(char* filename);

#endif