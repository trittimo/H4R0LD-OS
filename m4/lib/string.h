#ifndef _STRING_H
#define _STRING_H

int strcmp(char* str1, char* str2);
int strncmp(char* str1, char* str2, int len);
int strpos(char c, char* str);
int strlen(char* str);
int itostring_c(char* buffer, int value, int base, int written);
int itostring(char* buffer, int value, int base);

#endif