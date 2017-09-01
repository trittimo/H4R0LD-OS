#include "./string.h"

int strcmp(char* str1, char* str2) {
  int i;
  for (i = 0; str1[i] == str2[i]; i++) {}
  if (str1[i-1] == 0) {
    return 1;
  }
  return 0;
}

int strncmp(char* str1, char* str2, int len) {
  int i;
  for (i = 0; str1[i] == str2[i] && i < len; i++) {}
  if (i == len) {
    return 1;
  }
  return 0;
}
