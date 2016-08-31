#include <stdio.h>

int main() {
  for (int i = 0; i < 130; i++) {
    printf("i: %d, c: ", i);
    putchar(i);
    printf("\n");
  }
}
