/*
  Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
  Team L
*/
#include "./math.h"

int mod(int a, int b) {
  while (a >= b) {
    a = a - b;
  }
  return a;
}

int div(int a, int b) {
  int quotient = 0;
  while ((quotient + 1) * b  <= a) {
    quotient++;
  }
  return quotient;
}
