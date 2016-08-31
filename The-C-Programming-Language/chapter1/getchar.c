#include <stdio.h>

/**
 * Version 2
 */
int main() {
  int c;

  while ((c = getchar()) != EOF)
    putchar(c);
}

/**
 * Version 1
 */
/*int main() {
  int c;

  c = getchar();
  while (c != EOF) {
    putchar(c);
    c = getchar();
  }
}*/
