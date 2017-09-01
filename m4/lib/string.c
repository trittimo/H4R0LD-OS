#include "./string.h"
#include "./math.h"

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
  for (i = 0; str1[i] == str2[i] && i < len; i++) {
    if (str1[i] == 0 && str2[i] == 0) {
      return 1;
    }
  }
  if (i == len) {
    return 1;
  }
  return 0;
}

int strpos(char c, char* str) {
  int i;
  for (i = 0; ; i++) {
    if (str[i] == 0) {
      return -1;
    } else if (str[i] == c) {
      return i;
    }
  }
}

int strlen(char* str) {
  int i;
  for (i = 0; i < 512; i++) {
    if (str[i] == 0) {
      break;
    }
  }
  return i;
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
