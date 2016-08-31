#include <stdio.h>

#define LO     32  // lowest character in the alphabet (space)
#define HI     126 // highest character in the alphabet (~)
#define RANGE  94  // difference between HI and LO

int main() {
  int nchar[RANGE];
  for (int i = 0; i < RANGE; i++)
    nchar[i] = 0;

  int c;
  while ((c = getchar()) != EOF) {
    if (c >= LO && c <= HI) {
      ++nchar[c-LO];
    }
  }

  for (int i = 0; i < RANGE; i++) {
    if (nchar[i] > 0) {
      putchar(i+LO);
      printf(": ");
      for (int j = 0; j < nchar[i]; j++)
        printf("#");
      printf("\n");
    }
  }
}
