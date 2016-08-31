#include <stdio.h>

int main() {
  int c;

  // keep getting chars until we hit the end of the file
  while ((c = getchar()) != EOF) {
    // if the char is a blank, enter a new state
    if (c == ' ') {
      // as long as we see blanks, keep moving on
      while ((c = getchar()) == ' ');
      // then put a single blank to replace all the extra blanks
      putchar(' ');
      // if we hit the EOF character, break the outer while loop,
      // to avoid putchar putting it onto the screen
      if (c == EOF) break;
    }

    putchar(c);
  }
}
