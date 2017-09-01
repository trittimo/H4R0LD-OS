/* Project by: Michael Trittin, RJ DeCramer, Jaron Goodman */
#include "./print.h"

#define VID_MEM_SEGMENT 0xB000
#define VID_MEM_OFFSET 0x8000

void clear() {
  int x, y;
  for (y = 0; y < 25; y++) {
    for (x = 0; x < 80; x++) {
      print(" ", 1, x, y);
    }
  }
}

/**
 * Screen is 80 characters by 25 lines
 * @param string [description]
 * @param length [description]
 * @param x      [description]
 * @param y      [description]
 */
void print(char* string, int length, int x, int y) {
  int i, offset;
  offset = 80 * y;
  offset *= 2;
  offset += x * 2;
  offset += VID_MEM_OFFSET;
  for (i = 0; i < length; i++) {
    putInMemory(VID_MEM_SEGMENT, offset+(i*2), string[i]);
    putInMemory(VID_MEM_SEGMENT, offset+(i*2)+1, 0x7);
  }
}
