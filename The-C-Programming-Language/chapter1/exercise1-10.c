#include <stdio.h>

int main() {
  int c;

  while ((c = getchar()) != EOF) {
    switch (c) {
      case '\t':
        printf("\\t");
        break;
      case '\b':
        printf("\\b");
        break;
      case '\\':
        printf("\\\\");
        break;
      case '\n':
        printf("\\n");
        break;
      default:
        putchar(c);
    }
  }
}
