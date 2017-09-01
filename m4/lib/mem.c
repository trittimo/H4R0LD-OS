#include "./mem.h"

void zero(char* buf, int len) {
  int i;
  for (i = 0; i < len; i++) {
    buf[i] = 0;
  }
}