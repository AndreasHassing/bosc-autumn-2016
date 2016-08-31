#include <stdio.h>

int main() {
  int c, newlines = 0, tabs = 0, blanks = 0;

  while ((c = getchar()) != EOF) {
    switch (c) {
      case '\t':
        tabs++;
        break;
      case '\n':
        newlines++;
        break;
      case ' ':
        blanks++;
        break;
    }
  }

  printf("Newlines: %d\nTabs: %d\nBlanks: %d\n", newlines, tabs, blanks);
}
