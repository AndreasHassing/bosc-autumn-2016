#include <assert.h>
#include <stdio.h>

int main() {
  int c;

  while ((c = getchar()) != EOF) {
    assert((c != EOF) == 1);
    putchar(c);
  }

  assert((c != EOF) == 0);
}
